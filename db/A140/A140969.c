/*
 * A140969  Prime numbers whose hexadecimal representation uses only the
 *          digits A,B,C,D,E,F (and not the decimal digits).
 * https://oeis.org/A140969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime whose hexadecimal representation uses only
 *            the digits A,B,C,D,E,F (i.e. all nibbles are ≥ 10)
 * Range:     u8 n <= 7, u16 n <= 129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9630 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return true iff all hexadecimal nibbles of x are in {10,…,15}. */
static bool hex_allowed(idx_t x)
{
    if (x == 0)
        return false;
    while (x)
    {
        unsigned nib = (unsigned)(x & 0xF);
        if (nib < 10)   /* nibble < 'A' */
            return false;
        x >>= 4;
    }
    return true;
}

/* A fixed-size cache large enough for n ≈ 10000.
   The 10000th such prime is < 2³⁵, so it comfortably fits in seq_t. */
#define CACHE_CAP 20000
static seq_t prime_cache[CACHE_CAP];
static size_t cache_len   = 0;   /* number of cached primes */
static idx_t  last_tested = 0;   /* last integer examined */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* offset is 1 – value for n==0 is irrelevant */

    /* Return a cached value if already known. */
    if (n <= cache_len)
        return prime_cache[n - 1];

    /* Search forward until we collect the n‑th prime. */
    idx_t cur = last_tested + 1;
    while (1)
    {
        if (hex_allowed(cur) && is_prime_big((seq_t)cur))
        {
            if (cache_len < CACHE_CAP)
                prime_cache[cache_len++] = (seq_t)cur;
            last_tested = cur;
            if (n <= cache_len)
                return prime_cache[n - 1];
        }
        ++cur;
    }
}

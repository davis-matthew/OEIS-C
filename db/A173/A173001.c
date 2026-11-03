/*
 * A173001  Values of n such that exactly one of 5n+1 and 5n^2+5n+1 is prime.
 * https://oeis.org/A173001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A173001.
   a(n) is the n-th natural number m such that exactly one of
   5*m + 1 and 5*m² + 5*m + 1 is prime. */
#include <stdbool.h>
#include <stdlib.h>

/* Prime test provided by the utils. */
#include <stdint.h>
#include "../utils/mem.h"
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

/* Static cache of already found terms. */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_test = 0;      /* next natural number to examine */

/* Resizes the cache if necessary. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache)
        abort();                    /* Out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialize cache on first use. */
    if (!cache)
    {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            abort();                /* Out of memory */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        ++next_test;                   /* m = next natural number to test */
        seq_t m = (seq_t)next_test;

        /* Compute 5*m + 1 */
        seq_t f1 = (seq_t)5 * m + (seq_t)1;

        /* Compute 5*m^2 + 5*m + 1 */
        seq_t f2 = (seq_t)5 * m * m + (seq_t)5 * m + (seq_t)1;

        bool p1 = is_prime_big(f1);
        bool p2 = is_prime_big(f2);

        if (p1 ^ p2)                   /* exactly one is prime */
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];                /* a(1) is the first element */
}

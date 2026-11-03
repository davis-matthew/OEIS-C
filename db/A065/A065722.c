/*
 * A065722  Primes that when written in base 4, then reinterpreted in base 10,
 *          again give primes.
 * https://oeis.org/A065722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that interpreting the * base‑4
 *            representation of p as a decimal integer yields another * prime.
 *            The sequence is defined for n ≥ 1
 * Range:     u8 n <= 13, u16 n <= 130, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 897 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* Convert a prime p (idx_t) to the integer obtained by reading its
 * base‑4 digits as decimal digits.  Return type seq_t. */
static seq_t conv_base4_to_decimal(idx_t p)
{
    unsigned char digits[256];
    size_t len = 0;
    seq_t v = (seq_t)p;
    while (v != 0) {
        digits[len++] = (unsigned char)(v % 4);  /* 0..3 */
        v = v / 4;
    }
    if (len == 0) return 0;   /* should not happen for a prime */
    seq_t res = 0;
    for (size_t i = len; i-- > 0; ) {
        res = res * 10 + digits[i];
    }
    return res;
}

static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t primes_checked = 1;  /* last integer examined */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap >= (size_t)needed) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* sequence offset is 1 */
    while (cache_len < n) {
        /* Find next prime number. */
        do { ++primes_checked; } while (!is_prime(primes_checked));

        /* Convert to decimal via base‑4 digits. */
        seq_t conv = conv_base4_to_decimal(primes_checked);

        /* Check primality of the converted value. */
        if (is_prime_big(conv)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)primes_checked;
        }
    }
    return seq_cache[n-1];
}

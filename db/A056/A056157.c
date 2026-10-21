/*
 * A056157  Primes p whose period of reciprocal equals (p-1)/4.
 * https://oeis.org/A056157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p whose decimal reciprocal
   has period (p-1)/4, i.e. the multiplicative order of 10 modulo p
   equals (p-1)/4.  The primes are cached so that calls may arrive
   in any order. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

/* Test whether a prime p satisfies ord_p(10) = (p-1)/4. */
static bool has_correct_order(uint64_t p)
{
    /* p must be 1 mod 4 and greater than 5. */
    if (p <= 5 || (p & 3) != 1)
        return false;

    uint64_t e = (p - 1) / 4;
    seq_t modulo = (seq_t)p;
    if (mod_pow((seq_t)10, e, modulo) != (seq_t)1)
        return false;                       /* period does not divide e */

    uint64_t m = e;
    /* test each prime factor of e */
    for (uint64_t f = 2; f * f <= m; ++f) {
        if (m % f == 0) {
            /* remove all copies of f */
            while (m % f == 0)
                m /= f;
            if (mod_pow((seq_t)10, e / f, modulo) == (seq_t)1)
                return false;               /* order is smaller than e */
        }
    }
    if (m > 1) { /* remaining prime factor */
        if (mod_pow((seq_t)10, e / m, modulo) == (seq_t)1)
            return false;
    }
    return true;
}

/* Cache of found terms. */
static seq_t  *cache      = NULL;   /* array of seq_t */
static size_t cache_len  = 0;       /* number of terms stored */
static size_t cache_cap  = 0;       /* capacity of the array */
static uint64_t last_checked = 1;   /* last prime examined */

/* Public function: returns the n-th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_checked);
        last_checked = p;

        if (has_correct_order(p)) {
            /* Ensure capacity for one more element. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}

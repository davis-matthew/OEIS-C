/*
 * A088709  Numbers m that are a product of two primes j and k such that m+j+k
 *          is prime.
 * https://oeis.org/A088709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m that is a product of two primes j and
 *            k such that m + j + k is prime. Computed by a simple search
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* External types supplied by the build */
typedef idx_t idx_t;   /* defined elsewhere */
typedef seq_t seq_t;   /* defined elsewhere */

/* Cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Current value to test for semiprime */
static uint64_t current_m = 6;   /* first possible semiprime */

/* Grow the cache when needed */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Test whether a uint64_t value is a product of exactly two primes and
   whether the sum with its prime factors is prime.                          */
static bool candidate(uint64_t m, uint64_t *p_out, uint64_t *q_out)
{
    /* Find smallest prime factor p of m (trial division).  If none is found,
       m is prime.  */
    uint64_t p = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            p = d;
            break;
        }
    }
    if (p == 0)          /* m is prime */
        return false;

    uint64_t q = m / p;
    if (!is_prime_big((seq_t)q))
        return false;    /* not a semiprime with two primes */

    /* Check if m + p + q is prime.  Use 128‑bit arithmetic for safety. */
    unsigned __int128 sum128 = (unsigned __int128)m + p + q;
    seq_t sum = (seq_t)sum128;
    if (!is_prime_big(sum))
        return false;

    *p_out = p;
    *q_out = q;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but guard */

    /* Ensure cache has enough space for n terms */
    if (cache_len < n) {
        if (cache_cap < n) {
            while (cache_cap < n)
                grow_cache();
        }

        /* Generate new terms until cache contains n elements */
        while (cache_len < n) {
            uint64_t p, q;
            /* Find next candidate */
            while (true) {
                uint64_t m = current_m++;
                if (m < 6)          /* skip numbers < 6 (no semiprime there) */
                    continue;
                if (candidate(m, &p, &q)) {
                    cache[cache_len++] = (seq_t)m;
                    break;          /* move on to next required term */
                }
            }
        }
    }

    return cache[n - 1];
}

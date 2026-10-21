/*
 * A211337  Numbers k for which the number of divisors, tau(k), is congruent
 *          to 1 modulo 3.
 * https://oeis.org/A211337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k with τ(k) ≡ 1 (mod 3)
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Count the number of divisors of n using trial division. */
static idx_t divisor_count_u64(idx_t n)
{
    idx_t m = n;
    idx_t tau = 1;
    if (m == 0) return 0;                 /* not used, guard */

    /* factor 2 separately */
    if ((m & 1) == 0) {
        idx_t exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++exp;
        }
        tau *= (exp + 1);
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    if (m > 1) tau *= 2;   /* remaining prime factor */
    return tau;
}

/* Dynamic cache for already computed terms. */
static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;
static idx_t  cache_cap  = 0;
static idx_t  next_candidate = 1;   /* first integer to examine */

/* Grow the cache when needed. */
static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0 */

    /* Fill cache until we have the required term. */
    while (cache_len < n) {
        idx_t tau = divisor_count_u64(next_candidate);
        if (tau % 3 == 1) {
            grow_cache();
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

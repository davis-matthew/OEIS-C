/*
 * A081306  Numbers k with prime factors less than 2*lpf(k), where lpf(k) is
 *          the least prime factor of k (A020639).
 * https://oeis.org/A081306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that * every prime
 *            factor p of k satisfies p < 2*lpf(k), where lpf(k) * is the
 *            least prime factor of k
 * Range:     u8 n <= 99, u16 n <= 7691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/lpf.h"   /* provides: idx_t lpf(idx_t n) */

/* Check whether k satisfies the condition.
 * k is a 64‑bit unsigned integer (idx_t is uint64_t). */
static bool passes(idx_t k)
{
    if (k == 1) return true;

    idx_t lpf_val = lpf(k);          /* smallest prime factor of k    */
    idx_t limit   = lpf_val * 2;      /* primes must be < limit       */

    idx_t tmp = k;
    while (tmp > 1) {
        idx_t p = lpf(tmp);           /* next prime factor of tmp     */
        if (p >= limit) return false;
        /* strip all copies of p from tmp                 */
        while (tmp % p == 0) tmp /= p;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safeguard for non‑positive indices */

    /* Memoisation state shared across calls. */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;
    static size_t   count     = 0;
    static idx_t    candidate = 1;   /* next integer to test             */

    /* Grow cache when needed. */
    while (count < (size_t)n) {
        if (passes(candidate)) {
            if (count >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* allocation failed */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[count++] = (seq_t)candidate;
        }
        ++candidate;   /* move to following integer */
    }

    return cache[(size_t)n - 1];
}

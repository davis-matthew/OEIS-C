/*
 * A057686  Trajectory of 23 under the `23x+1' map: x -> x/p if p = least
 *          prime factor < 23, else x*23 + 1.
 * https://oeis.org/A057686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A057686 – Trajectory of 23 under the 23x+1 map.
 *
 *  a(0) = 23
 *  For n ≥ 1:
 *      let x = a(n−1)
 *      let p = the least prime factor of x that is < 23
 *          (consider 2,3,5,7,11,13,17,19 in that order)
 *      if such a p exists then a(n) = x / p
 *      otherwise a(n) = 23*x + 1
 *
 *  The sequence is eventually 5‑periodic (a(n) = a(n−5) for n > 18),
 *  but the algorithm below computes each term directly and caches the
 *  results.  It works for the arbitrary‑precision build (seq_t is a
 *  C++ class) and for all fixed‑width integer builds.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache for computed terms */
static idx_t cache_len = 0;      /* number of cached entries (indices 0 … cache_len-1) */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices 0 … n */
    if (n + 1 > cache_len) {
        /* size in bytes for the new allocation */
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);

        /* realloc with explicit cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_size);
        if (!tmp) {
            /* allocation failed – return 0 as a sentinel value */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms from the previous length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)23;   /* initial term */
                continue;
            }

            seq_t prev = cache[i - 1];
            seq_t next;
            int found = 0;

            /* primes to test, in increasing order */
            static const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
            static const size_t prime_cnt = sizeof(primes) / sizeof(primes[0]);

            for (size_t k = 0; k < prime_cnt; ++k) {
                seq_t p = (seq_t)primes[k];
                if (prev % p == (seq_t)0) {
                    next = prev / p;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                /* No prime factor < 23 found → apply 23x+1 rule */
                next = prev * (seq_t)23 + (seq_t)1;
            }

            cache[i] = next;
        }

        cache_len = n + 1;   /* update cache length */
    }

    return cache[n];
}

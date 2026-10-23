/*
 * A194422  Numbers m such that Sum_{k=1..m} (<2/3 + k*r> - <k*r>) < 0, where
 *          r=sqrt(2) and < > denotes fractional part.
 * https://oeis.org/A194422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 2687, u32 n <= 2687, u64 n <= 2687,
 *            u128 n <= 2687, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194422: Numbers m such that
   Sum_{k=1..m} ( frac(k*sqrt(2)+2/3) - frac(k*sqrt(2)) ) < 0
   The test reduces to: let N1 be the count of 1 ≤ k ≤ m with frac(k*sqrt(2)) ≤ 1/3.
   Then the condition is 3*N1 < m.  This function returns the n‑th such m,
   with a(1)=1, a(2)=2, ... .
   The implementation scans m sequentially, keeps a running count
   of N1, and stores found terms in a static cache for later reuse.
*/

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;    /* array of a(k) */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* State used to scan forward in m */
    static idx_t processed_m = 0;  /* current m processed */
    static uint64_t N1 = 0;        /* number of k ≤ processed_m with frac(k*sqrt2) ≤ 1/3 */

    /* Pre‑compute sqrt(2) once */
    static const long double sq2 = sqrtl(2.0L);
    static const long double one_third = 1.0L / 3.0L;
    static const long double eps = 1e-15L; /* safety buffer for rounding */

    /* Ensure cache exists */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* graceful failure */
    }

    /* Keep computing until we have n terms */
    while (cache_len < n) {
        ++processed_m;

        /* Fractional part of processed_m * sqrt(2) */
        long double t = processed_m * sq2;
        long double frac = t - floorl(t);

        /* Increment N1 if fractional part ≤ 1/3 (with tiny epsilon) */
        if (frac <= (one_third + eps)) {
            ++N1;
        }

        /* Check condition 3*N1 < processed_m */
        if ((uint64_t)3 * N1 < (uint64_t)processed_m) {
            /* Grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* On allocation failure, return 0 */
                    free(cache);
                    cache = NULL;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)processed_m;
            ++cache_len;
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}

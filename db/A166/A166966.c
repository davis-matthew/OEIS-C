/*
 * A166966  Eigensequence of A047999, Sierpinski's gasket.
 * https://oeis.org/A166966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Define v[0] = 1. For m ≥ 1, v[m] = Σ v[k] over all k that are
 *            submasks of (m – 1), i.e. over all k such that (k & ~(m – 1)) ==
 *            0. Then a(n) = v[n + 1]. This recurrence comes from the
 *            eigensequence of the shifted Sierpinski triangle (A047999)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache of computed v[ ] values    */
    static idx_t cache_len = 0;     /* number of entries currently stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache     */

    /* We need to compute v[0 .. needed] where needed = n + 1 */
    idx_t needed = n + 1;

    /* Ensure cache has enough capacity */
    if (needed >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)(needed + 1)) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0; this situation should not
               occur in the typical test harness. */
            return 0;
        }
        /* Zero‑initialize the new part */
        if (new_cap > cache_cap) {
            memset(tmp + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base case if not done yet */
    if (cache_len == 0) {
        cache[0] = 1;            /* v[0] = 1 */
        cache_len = 1;
    }

    /* Compute any missing v[m] values up to 'needed' */
    for (idx_t m = cache_len; m <= needed; ++m) {
        /* m ≥ 1 here */
        idx_t mask = m - 1;      /* submasks of m-1 are the subsets */
        seq_t sum = 0;
        if (mask == 0) {
            sum = cache[0];      /* only submask is 0 */
        } else {
            idx_t sub = mask;
            while (1) {
                sum += cache[sub];
                if (sub == 0) break;
                sub = (sub - 1) & mask;
            }
        }
        cache[m] = sum;
    }

    /* Update the stored length */
    cache_len = needed + 1;

    /* Return the requested term a(n) = v[n+1] */
    return cache[needed];
}

/*
 * A170467  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational growth‑series
   (t^45 + 2*t^44 + 2*t^43 + … + 2*t + 1)
/   (300*t^45 - 24*t^44 - 24*t^43 - … - 24*t + 1).
   Let a(0)=1.  For 1 ≤ n ≤ 44:
        a(n) = 2 + 24 * Σ_{i=1}^{n}  a(n-i)
   For n ≥ 45:
        a(n) = 1 + 24 * Σ_{i=1}^{44} a(n-i) – 300 * a(n-45)
   The array of values is cached across calls. */
#ifndef LIMIT
#define LIMIT 10000          /* assumed maximal n in tests – increases safety */
#endif

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;          /* number of terms currently stored */

    /* enlarge the cache if necessary */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {                 /* allocation failed – return 0 */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_len = new_len;

        /* initialise the first term if it has not been set yet */
        if (old_len == 0) {
            cache[0] = (seq_t)1;          /* a(0) = 1 */
            old_len = 1;
        }

        /* compute missing terms up to n */
        for (idx_t j = old_len; j <= n; ++j) {
            seq_t bi = (j == 0) ? (seq_t)1 : ((j <= 44) ? (seq_t)2 : (seq_t)1);
            seq_t sum = (seq_t)0;
            idx_t maxk = (j <= 44) ? j : (idx_t)44;
            for (idx_t k = 1; k <= maxk; ++k)
                sum += cache[j - k];
            seq_t val = bi + (seq_t)24 * sum;
            if (j >= 45)
                val -= (seq_t)300 * cache[j - 45];
            cache[j] = val;
        }
    }

    return cache[n];
}

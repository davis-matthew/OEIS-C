/*
 * A170327  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170327
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

/* Recurrence:
   a(0) = 1.
   For 1 <= n <= 41:
       a(n) = 2 + 28 * (a(n-1) + a(n-2) + ... + a(0))
   For n >= 42:
       a(n) = 28 * (a(n-1) + a(n-2) + ... + a(n-41)) -
              406 * a(n-42)
   The sequence is memoised so that each term is computed only once,
   regardless of the order of calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t cache_len = 0;      /* number of values stored  [0, len-1] */
    static idx_t cache_cap = 0;      /* allocated capacity of the array   */

    /* enlarge the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t m = cache_len; m <= n; ++m) {

        if (m == 0) {
            cache[0] = 1;
        }
        else if (m <= 41) {          /* short‑recurrence for the first 41 terms */
            seq_t sum = 0;
            for (idx_t j = 1; j <= m; ++j)
                sum += cache[m - j];
            cache[m] = 2 + sum * (seq_t)28;
        }
        else {                       /* full linear recurrence for n >= 42 */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 41; ++j)
                sum += cache[m - j];
            cache[m] = sum * (seq_t)28 -
                       cache[m - 42] * (seq_t)406;
        }
    }
    cache_len = n + 1;
    return cache[n];
}

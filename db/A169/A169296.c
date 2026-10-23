/*
 * A169296  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169296: number of reduced words of length n in a Coxeter group
   on 43 generators with relations S_i^2 = (S_i S_j)^29 = I.
   The ordinary generating function is
     (t^29 + 2*t^28 + … + 2*t + 1) /
     (1 - 41*t - 41*t^2 - … - 41*t^28 + 861*t^29).
   Hence the sequence satisfies the recurrence
      a[0] = 1,
      a[n] = P[n] - Σ_{k=1}^{min(29,n)} q[k] * a[n-k],   n ≥ 1
   with
      q[0] = 1,  q[1..28] = -41,  q[29] = 861,
      P[n] = 1  if n==0 or n==29,
             2  if 1 ≤ n ≤ 28,
             0  otherwise.
   This recurrence generates all terms (the first 29 determine the rest).
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_max = (idx_t)-1; /* last computed index */

    /* Reallocate cache if needed */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (cache_cap == 0 ? 32 : cache_cap * 2);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed, terminate program */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* k from 1 to min(28,i) with coefficient -41 */
        for (idx_t k = 1; k <= 28 && k <= i; ++k) {
            sum += (seq_t)(-41) * cache[i - k];
        }
        /* k = 29, coefficient +861 */
        if (i >= 29) {
            sum += (seq_t)861 * cache[i - 29];
        }

        /* P[i] */
        seq_t P = 0;
        if (i == 0 || i == 29) {
            P = 1;
        } else if (i <= 28) {
            P = 2;
        }

        cache[i] = P - sum;
    }

    if (n > cache_max) cache_max = n;
    return cache[n];
}

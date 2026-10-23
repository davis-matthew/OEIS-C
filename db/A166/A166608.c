/*
 * A166608  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the generating function:
 * a(0) = 1
 * for 1 ≤ n ≤ 11:
 *      a(n) = 2 + 20 * Σ_{k=0}^{n-1} a(k)
 * for n = 12:
 *      a(12) = 20 * Σ_{k=1}^{11} a(k) - 210 * a(0) + 1
 * for n ≥ 13:
 *      a(n) = 20 * Σ_{k=n-11}^{n-1} a(k) - 210 * a(n-12)
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = (idx_t)(-1);

    /* initialize cache */
    if (cache == NULL) {
        cache_size = 256;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
    }
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = n + 1;
        if (new_size < 256) new_size = 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* realloc failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* static sums used while building the sequence */
    static seq_t sumPrev = 0;    /* Σ_{k=0}^{i-1} a(k), for i ≤ 11 */
    static seq_t sumLast11 = 0;  /* Σ_{k=i-11}^{i-1} a(k), for i ≥ 12 */
    static int initialized = 0;

    if (!initialized) {
        cache[0] = (seq_t)1;
        sumPrev = (seq_t)1;
        last_computed = 0;
        initialized = 1;
    }

    /* compute terms up to n if necessary */
    if (n > last_computed) {
        idx_t i;
        for (i = last_computed + 1; i <= n; ++i) {
            if (i <= 11) {  /* 1 ≤ i ≤ 11 */
                cache[i] = (seq_t)2 + (seq_t)20 * sumPrev;
                sumPrev += cache[i];
                if (i == 11) {
                    /* prepare sum of a(1) … a(11) for the next stage */
                    sumLast11 = sumPrev - cache[0];
                }
            } else if (i == 12) { /* special case for n = 12 */
                cache[i] = (seq_t)20 * sumLast11
                           - (seq_t)210 * cache[i - 12]
                           + (seq_t)1;    /* numerator coefficient at n=12 is 1 */
                sumPrev += cache[i];
                sumLast11 += cache[i] - cache[i - 11]; /* now sum a(2)…a(12) */
            } else {            /* i ≥ 13 */
                cache[i] = (seq_t)20 * sumLast11
                           - (seq_t)210 * cache[i - 12];
                sumPrev += cache[i];
                sumLast11 += cache[i] - cache[i - 11];
            }
        }
        last_computed = n;
    }

    return cache[n];
}

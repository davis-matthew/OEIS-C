/*
 * A170318  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=19*Σ_{i=1}^{41} a(n-i) – 190*a(n-42)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (With the convention a(k)=0 for k<0.  The first 42 terms are obtained
   by the same recurrence with the numerators 1,2,…,2,1 for t^0,…,t^42.
   This implements the rational generating function given in the OEIS
   entry and works for all n≥0 in O(n) time.  The series is memoised.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  computed = 0;   /* number of terms computed (cache[0..computed-1]) */
    static size_t cap    = 0;     /* allocated size of cache */

    /* enlarge cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t m = computed; m <= n; ++m) {
        if (m < 42) {
            /* N(m) = 1 for m==0, 2 otherwise           */
            seq_t N = (m == 0 ? (seq_t)1 : (seq_t)2);
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                sum += cache[m - k];
            }
            cache[m] = (seq_t)19 * sum + N;
        } else if (m == 42) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 41; ++k) {
                sum += cache[42 - k];
            }
            /* N(42) = 1 */
            cache[42] = (seq_t)19 * sum - (seq_t)190 * cache[0] + (seq_t)1;
        } else {            /* m > 42 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 41; ++k) {
                sum += cache[m - k];
            }
            cache[m] = (seq_t)19 * sum - (seq_t)190 * cache[m - 42];
        }
    }
    computed = n + 1;

    return cache[n];
}

/*
 * A140662  Number of possible column states for self-avoiding polygons in a
 *          slit of width n.
 * https://oeis.org/A140662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = A001006(n+1) - 1
   Recurrence (for n ≥ 5):
      a(n) = ((4n+7)*a(n-1) - (2n+3)*a(n-2) - (4n-5)*a(n-3) + 3*(n-2)*a(n-4)) / (n+3)
   Initial values:
      a(1) = 1, a(2) = 3, a(3) = 8, a(4) = 20
   Memoization is used to handle any access order efficiently. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* offset is 1; below offset not used */

    /* static cache: cache[i] holds a(i+1) for i starting at 0 */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;         /* number of precomputed values */

    /* ensure at least the first four terms are computed */
    if (cache == NULL) {
        cache_sz = 4;
        cache = (seq_t*)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */
        cache[0] = (seq_t)1;           /* a(1) */
        cache[1] = (seq_t)3;           /* a(2) */
        cache[2] = (seq_t)8;           /* a(3) */
        cache[3] = (seq_t)20;          /* a(4) */
    }

    /* grow cache if needed */
    if (cache_sz < n) {
        idx_t new_sz = n;
        cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        /* compute new terms using recurrence */
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 2];   /* a(i-1)  */
            seq_t a2 = cache[i - 3];   /* a(i-2)  */
            seq_t a3 = cache[i - 4];   /* a(i-3)  */
            seq_t a4 = cache[i - 5];   /* a(i-4)  */

            seq_t coeff1 = (seq_t)(4 * i + 7);
            seq_t coeff2 = (seq_t)(2 * i + 3);
            seq_t coeff3 = (seq_t)(4 * i - 5);
            seq_t coeff4 = (seq_t)(3 * (i - 2));

            seq_t num = coeff1 * a1;
            num -= coeff2 * a2;
            num -= coeff3 * a3;
            num += coeff4 * a4;

            seq_t denom = (seq_t)(i + 3);
            cache[i - 1] = num / denom;
        }
        cache_sz = n;
    }

    return cache[n - 1];
}

/*
 * A087674  Value of the n-th Eulerian polynomial (cf. A008292) evaluated at
 *          x=-2.
 * https://oeis.org/A087674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * k! * (-3)^(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static caches for the sequence values, the auxiliary B values,
       and the current row of T = Stirling2(n,k) * k!   */
    static seq_t *a_cache = NULL;
    static seq_t *B_cache = NULL;
    static seq_t *t = NULL;          /* current row of T */
    static idx_t cache_len = 0;      /* number of computed terms   */

    /* initialise for n = 0 */
    if (cache_len == 0) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        B_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = 1;        /* a(0) = 1 */
        B_cache[0] = 0;        /* B(0) = 0 */
        t = (seq_t*)malloc(sizeof(seq_t));
        t[0] = 1;              /* T(0,0) = 1 */
        cache_len = 1;
    }

    /* extend the cache up to n */
    if (n >= cache_len) {
        for (idx_t m = cache_len; m <= n; ++m) {
            /* increase the size of the current row to m+1 */
            t = (seq_t*)realloc(t, (size_t)(m + 1) * sizeof(seq_t));
            t[m] = 0;  /* new element, initially zero */

            /* compute T(m,k) from T(m-1,k)  (in place, descending order) */
            for (idx_t k = m; k > 0; --k) {
                t[k] = (seq_t)k * (t[k] + t[k - 1]);
            }
            t[0] = 0;  /* T(m,0) = 0 for m > 0 */

            /* compute a_m and B_m using the new row t */
            seq_t pow = 1;        /* (-3)^(m-k), starting with k = m */
            seq_t a_m  = 0;
            seq_t B_m  = 0;
            for (idx_t k = m; ; --k) {
                seq_t val = t[k];
                a_m += val * pow;
                B_m += (seq_t)k * val * pow;
                if (k == 0) break;
                pow = pow * (seq_t)(-3);
            }

            /* store the computed terms */
            a_cache = (seq_t*)realloc(a_cache, (size_t)(m + 1) * sizeof(seq_t));
            B_cache = (seq_t*)realloc(B_cache, (size_t)(m + 1) * sizeof(seq_t));
            a_cache[m] = a_m;
            B_cache[m] = B_m;
        }
        cache_len = n + 1;
    }

    return a_cache[n];
}

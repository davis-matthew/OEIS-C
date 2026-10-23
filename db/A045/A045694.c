/*
 * A045694  Number of ternary words of length n (beginning with 0) with
 *          autocorrelation function 2^(n-1).
 * https://oeis.org/A045694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(2*n) = 3*a(2*n-1) - a(n) (n >= 1) a(2*n+1) =
 *            3*a(2*n) (n >= 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* out of range, not expected */

    /* static cache used for memoisation */
    static seq_t *A      = NULL;
    static idx_t  size   = 0;         /* allocated size of A (>= n+1)    */
    static idx_t  last   = 0;         /* highest index already computed  */

    /* initialise cache on first call */
    if (size == 0) {
        size = 2;                     /* need indices 0 and 1 */
        A = (seq_t *)calloc(size, sizeof(seq_t));
        if (!A) return 0;             /* allocation failed */
        A[1] = 1;                     /* a(1) = 1 */
        last = 1;
    }

    /* grow cache if necessary */
    if (n >= size) {
        size = n + 1;
        A = (seq_t *)realloc(A, size * sizeof(seq_t));
        if (!A) return 0;             /* allocation failed */
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if ((i & 1) == 0) {           /* even index: i = 2*m */
            idx_t m = i >> 1;
            A[i] = 3 * A[i - 1] - A[m];
        } else {                      /* odd index:  i = 2*m+1 */
            A[i] = 3 * A[i - 1];
        }
    }
    last = n;
    return A[n];
}

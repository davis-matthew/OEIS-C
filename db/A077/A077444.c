/*
 * A077444  Numbers k such that (k^2 + 4)/2 is a square.
 * https://oeis.org/A077444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), with a(1)=2, a(2)=14 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;   /* memoization array  */
    static idx_t  sz = 0;     /* allocated capacity */
    static idx_t  cnt = 0;    /* number of terms stored */

    /* lazy initialization of first two terms */
    if (A == NULL) {
        sz = 4;                       /* small initial capacity */
        A = (seq_t*)malloc(sz * sizeof(seq_t));
        A[0] = (seq_t)2;              /* a(1) */
        A[1] = (seq_t)14;             /* a(2) */
        cnt = 2;
    }

    /* grow and compute missing terms if necessary */
    if (n > cnt) {
        if (n > sz) {
            sz = n;
            A = (seq_t*)realloc(A, sz * sizeof(seq_t));
        }
        for (idx_t i = cnt + 1; i <= n; ++i) {
            seq_t next = (seq_t)( (seq_t)6 * A[i - 2] - A[i - 3] );
            A[i - 1] = next;          /* store a(i) */
        }
        cnt = n;
    }

    return A[n - 1];
}

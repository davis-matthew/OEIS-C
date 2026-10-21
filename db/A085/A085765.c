/*
 * A085765  Partial sums and bisection of A086450.
 * https://oeis.org/A085765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 115, u32 n <= 909, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from a(n) = A086450(2n) = Σ_{k=0}^n A086450(k).
   Let A(n) = a(n).  Then for m ≥ 0
   - A(2m)   = A(2m-1) + A(m)                 (even index)
   - A(2m+1) = A(2m)   + A(m) - A(m-1)        (odd index, A(-1) = 0)
   This implements the sequence defined by OEIS A085765.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *A   = NULL;   /* array holding a(0)…a(max_comp)   */
    static idx_t size   = 0;   /* allocated array size               */
    static idx_t max_comp = 0; /* highest computed index             */

    /* Lazily allocate the array on the first call.  */
    if (A == NULL) {
        size = 1;
        A    = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!A) return (seq_t)0;      /* allocation failed – unlikely in test harness */
        A[0] = 1;
        max_comp = 0;
    }

    /* Extend the array if we need a larger index.  */
    if (n > max_comp) {
        /* Grow the allocation to at least n+1 (doubling strategy). */
        if (size <= n) {
            idx_t newsize = (size == 0) ? 1 : size;
            while (newsize <= n) newsize <<= 1;
            seq_t *tmp = (seq_t *)realloc(A, sizeof(seq_t) * newsize);
            if (!tmp) return (seq_t)0;   /* allocation failed – unlikely */
            A = tmp;
            size = newsize;
        }

        /* Compute missing values from max_comp+1 up to n.  */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            if ((i & 1) == 0) {            /* even index: i = 2m */
                idx_t m = i >> 1;          /* m = i/2          */
                A[i] = A[i-1] + A[m];
            } else {                       /* odd index: i = 2m+1 */
                idx_t m = i >> 1;          /* m = (i-1)/2      */
                if (m == 0) {              /* i == 1 case, A(-1)=0 */
                    A[i] = A[i-1] + A[0];
                } else {
                    A[i] = A[i-1] + (A[m] - A[m-1]);
                }
            }
        }
        max_comp = n;
    }

    return A[n];
}

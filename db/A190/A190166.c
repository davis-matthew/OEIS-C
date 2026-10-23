/*
 * A190166  Number of (1,0)-steps at levels 0,2,4,... in all peakless Motzkin
 *          paths of length n.
 * https://oeis.org/A190166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Linear recurrence of order 7: (-n+1)*a(n) +(3*n-4)*a(n-1)
 *            +2*(-n+1)*a(n-2) +3*(n-2)*a(n-3) +2*(-n+3)*a(n-4)
 *            +(3*n-8)*a(n-5) +(-n+3)*a(n-6)=0 Initial terms: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=6, a(5)=14 The recurrence is used for all n
 *            ≥ 6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values.  A[0..A_max] are already computed.  */
    static seq_t *A        = NULL;
    static idx_t  A_max   = (idx_t)-1;   /* maximum index computed so far   */
    static size_t A_alloc = 0;          /* allocated array size            */

    /* Ensure array large enough for index n.  */
    if (A == NULL) {
        /* First call – allocate exactly (n+1) slots.  */
        A_alloc = n + 1;
        A = (seq_t *)malloc(A_alloc * sizeof(seq_t));
        if (!A) return 0;   /* allocation failed – return 0 as sentinel */
        /* Set the known base values.   */
        if (A_alloc > 0)   A[0] = 0;
        if (A_alloc > 1)   A[1] = 1;
        if (A_alloc > 2)   A[2] = 2;
        if (A_alloc > 3)   A[3] = 3;
        if (A_alloc > 4)   A[4] = 6;
        if (A_alloc > 5)   A[5] = 14;
        /* Compute further terms up to n (if necessary).  */
        for (idx_t i = 6; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            /* Terms with positive coefficients */
            pos += (seq_t)(3 * i - 4)        * A[i - 1];
            pos += (seq_t)(3 * (i - 2))      * A[i - 3];
            pos += (seq_t)(3 * i - 8)        * A[i - 5];
            /* Terms with negative coefficients – summed into 'neg'. */
            neg += (seq_t)(2 * (i - 1))      * A[i - 2];
            neg += (seq_t)(2 * (i - 3))      * A[i - 4];
            neg += (seq_t)(i - 3)            * A[i - 6];
            A[i] = (pos - neg) / (seq_t)(i - 1);
        }
        A_max = n;
        return A[n];
    }

    /* If the requested index is already computed, return immediately. */
    if (n <= A_max) return A[n];

    /* Grow the array if necessary.  The size is increased exactly to (n+1). */
    if (n + 1 > A_alloc) {
        A = (seq_t *)realloc(A, (n + 1) * sizeof(seq_t));
        if (!A) return 0;   /* allocation failed – return 0 as sentinel */
        A_alloc = n + 1;
    }

    /* The base values 0…5 are already set from the first allocation. */
    /* Compute new values from A_max+1 up to n. */
    for (idx_t i = A_max + 1; i <= n; ++i) {
        if (i <= 5) {               /* Safety:  base values are known. */
            switch (i) {
                case 0: A[0] = 0; break;
                case 1: A[1] = 1; break;
                case 2: A[2] = 2; break;
                case 3: A[3] = 3; break;
                case 4: A[4] = 6; break;
                case 5: A[5] = 14; break;
            }
            continue;
        }
        seq_t pos = 0, neg = 0;
        pos += (seq_t)(3 * i - 4)        * A[i - 1];
        pos += (seq_t)(3 * (i - 2))      * A[i - 3];
        pos += (seq_t)(3 * i - 8)        * A[i - 5];
        neg += (seq_t)(2 * (i - 1))      * A[i - 2];
        neg += (seq_t)(2 * (i - 3))      * A[i - 4];
        neg += (seq_t)(i - 3)            * A[i - 6];
        A[i] = (pos - neg) / (seq_t)(i - 1);
    }

    A_max = n;
    return A[n];
}

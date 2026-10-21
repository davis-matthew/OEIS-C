/*
 * A060237  a(n) = n!^2 * Sum_{m=1..n}( Sum_{k=1..m} 1/(k*m) ).
 * https://oeis.org/A060237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3n^2‑3n+1)a(n‑1) - 3(n‑1)^4 a(n‑2) + (n‑2)^3 (n‑1)^3
 *            a(n‑3) with initial values a(1)=1, a(2)=7, a(3)=85
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation of previously computed terms */
    static idx_t max_n = 0;       /* largest n for which A has been filled */
    static seq_t *A   = NULL;    /* array of computed terms, indexed by n */

    /* initialise for n ≥ 1 */
    if (max_n == 0)
    {
        max_n = 3;                                   /* we need indices 1..3 */
        A = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
        if (!A) return (seq_t)0; /* allocation failure */

        A[0] = (seq_t)0;      /* unused, keep zero */
        A[1] = (seq_t)1;      /* a(1) */
        A[2] = (seq_t)7;      /* a(2) */
        A[3] = (seq_t)85;     /* a(3) */
    }

    /* extend the table up to the requested n */
    if (n > max_n)
    {
        /* reallocate the array */
        seq_t *tmp = (seq_t *)realloc(A, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        A = tmp;

        /* compute new terms from max_n + 1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            /* several intermediate seq_t values */
            seq_t nn   = (seq_t)i;
            seq_t nn2  = nn * nn;                   /* n^2            */
            seq_t coeff1 = nn2 * 3 - nn * 3 + 1;    /* 3n^2-3n+1 */

            /* (n‑1)^4 */
            seq_t nm1   = nn - 1;
            seq_t nm1sq = nm1 * nm1;
            seq_t nm1pow4 = nm1sq * nm1sq;          /* (n‑1)^4 */

            /* (n‑2)^3 and (n‑1)^3 */
            seq_t nm2   = nn - 2;
            seq_t nm2sq = nm2 * nm2;
            seq_t nm2c   = nm2sq * nm2;             /* (n‑2)^3 */
            seq_t nm1c   = nm1sq * nm1;             /* (n‑1)^3 */

            /* terms of the recurrence */
            seq_t term1 = A[i-1] * coeff1;                 /* a(n‑1)*(3n²‑3n+1) */
            seq_t term2 = A[i-2] * (seq_t)3 * nm1pow4;      /* 3(n‑1)⁴ a(n‑2)   */
            seq_t term3 = A[i-3] * (nm2c * nm1c);          /* (n‑2)³(n‑1)³ a(n‑3) */

            /* a(i) = term1 - term2 + term3 */
            A[i] = term1 - term2 + term3;
        }
        max_n = n;
    }

    return A[n];
}

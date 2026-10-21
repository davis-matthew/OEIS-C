/*
 * A205543  Logarithmic derivative of the Bell numbers (A000110).
 * https://oeis.org/A205543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A205543 (Logarithmic derivative of the Bell numbers)
 *
 * Formula: Let B_n denote the Bell numbers, B_0 = 1 and
 * B_n = sum_{k=0}^{n-1} C(n-1,k) * B_k  (n >= 1).
 *
 * Then a(n) is given by
 *   a(n) = n * B_n - sum_{k=1}^{n-1} a(k) * B_{n-k}    (n >= 1),
 * where a(0) = 0.  The arrays B and a are computed on demand
 * and memoised for subsequent calls.
 */

#include <stdlib.h>

static seq_t *B   = NULL; /* Bell numbers */
static seq_t *A   = NULL; /* Sequence A205543 */
static idx_t   max_n = 0; /* Highest n for which B and A are known */

static seq_t *prevS = NULL; /* Stirling numbers S(n-1, *) for current n */
static idx_t   prevS_size = 0;

/* Ensure B and A are computed up to index n (inclusive) */
static void ensure_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    /* Extend storage for B and A */
    B = (seq_t *)realloc(B, (n + 1) * sizeof(seq_t));
    A = (seq_t *)realloc(A, (n + 1) * sizeof(seq_t));

    /* Initialise for n=0 if this is the first call */
    if (max_n == 0) {
        B[0] = (seq_t)1;
        A[0] = (seq_t)0;
        prevS = (seq_t *)malloc(sizeof(seq_t) * 1);
        prevS[0] = (seq_t)1;            /* S(0,0) = 1 */
        prevS_size = 1;
    }

    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* Allocate current Stirling row S(m,*) */
        seq_t *curS = (seq_t *)malloc((m + 1) * sizeof(seq_t));
        curS[0] = (seq_t)0;
        curS[m] = (seq_t)1;            /* S(m,m) = 1 */

        /* Recurrence: S(m,k) = k * S(m-1,k) + S(m-1,k-1)  for 1 <= k <= m-1 */
        for (idx_t k = 1; k < m; ++k) {
            seq_t t = prevS[k];
            t *= (seq_t)k;
            curS[k] = t + prevS[k - 1];
        }

        /* Compute Bell number B[m] = sum_{k=0}^{m} S(m,k) */
        seq_t sumB = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k)
            sumB += curS[k];
        B[m] = sumB;

        /* Compute a[m] = m * B[m] - sum_{k=1}^{m-1} a[k] * B[m-k] */
        seq_t sumProd = (seq_t)0;
        for (idx_t k = 1; k < m; ++k)
            sumProd += A[k] * B[m - k];
        A[m] = (seq_t)m * B[m] - sumProd;

        /* Prepare for next iteration */
        free(prevS);
        prevS = curS;
        prevS_size = m + 1;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* Out of range of defined offset */
    ensure_up_to(n);
    return A[n];
}

/*
 * A075502  Triangle read by rows: Stirling2 triangle with scaled diagonals
 *          (powers of 7).
 * https://oeis.org/A075502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 7^(r-m) * S(r,m) where S(r,m) are Stirling numbers of the
 *            second kind and the indices r,m are obtained from the triangular
 *            numbering of the lower‑triangular matrix: n =
 *            1,2,3,4,5,6,7,8,9,... r*m: 1,(1,2,3),(1,2,3,4,5,6), … m = n -
 *            (r-1)*r/2 with r*(r+1)/2 ≥ n > (r-1)*r/2. Recurrence for S:...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 47, u64 n <= 139, u128 n <= 411,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies the following typedefs:
 *   typedef uint64_t idx_t;   // index type
 *   typedef … seq_t;         // integer type (fixed or arbitrary‑precision)
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free  */
#if defined(__cplusplus)
#include <new>        /* new[], delete[] for C++ */
#endif

/* Fast exponentiation of 7, exponent fits in size_t */
static seq_t pow7_exp(size_t exp)
{
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < exp; ++i)
        result *= (seq_t)7;
    return result;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1; return 0 for safety */
        return (seq_t)0;

    /* --- determine row r and column m from triangular indexing --- */
    size_t r = 1;
    while ((idx_t)r * ((idx_t)r + 1) / 2 < n)
        ++r;
    idx_t prev_tri = (idx_t)(r - 1) * r / 2;
    size_t m = (size_t)(n - prev_tri);      /* 1 ≤ m ≤ r */

    /* --- allocate two rows of Stirling numbers, initialize to 0 --- */
    seq_t *prev = NULL;
    seq_t *curr = NULL;
#if defined(__cplusplus)
    prev = new seq_t[r + 1]();
    curr = new seq_t[r + 1]();
#else
    prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (prev && curr)
        for (size_t i = 0; i <= r; ++i) {
            prev[i] = (seq_t)0;
            curr[i] = (seq_t)0;
        }
#endif
    if (!prev || !curr) {   /* allocation failure */
#if defined(__cplusplus)
        delete[] prev;
        delete[] curr;
#else
        free(prev);
        free(curr);
#endif
        return (seq_t)0;
    }

    /* Base case: S(0,0)=1 */
    prev[0] = (seq_t)1;

    /* Build rows 1..r */
    for (size_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)0;               /* S(i,0)=0 for i>0 */
        for (size_t k = 1; k <= i; ++k) {
            seq_t term1 = prev[k];            /* may be 0 for k>i-1, safe */
            term1 *= (seq_t)k;                /* k * S(i-1,k) */
            seq_t term2 = prev[k - 1];        /* S(i-1,k-1) */
            curr[k] = term1 + term2;          /* S(i,k) */
        }
        if (i == r)                          /* final row obtained */
            break;
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* curr holds S(r, m); compute the triangle entry */
    seq_t result = curr[m] * pow7_exp(r - m);

    /* --- cleanup --- */
#if defined(__cplusplus)
    delete[] prev;
    delete[] curr;
#else
    free(prev);
    free(curr);
#endif

    return result;
}

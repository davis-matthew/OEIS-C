/*
 * A236311  Riordan array ((1-x)/(1-3*x+3*x^2), x/(1-3*x+3*x^2)).
 * https://oeis.org/A236311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A236311 – a(n) is the entry of the Riordan array
 *  ((1-x)/(1-3*x+3*x²), x/(1-3*x+3*x²)) enumerated row‑major.
 *
 *  Let T(n,k) be that array. Then
 *      T(0,0) = 1
 *      T(1,0) = 2 , T(1,1) = 1
 *      T(n,k) = 3*T(n-1,k) + T(n-1,k-1) - 3*T(n-2,k)   (0 ≤ k ≤ n)
 *      T(n,k) = 0   if k<0 or k>n
 *
 *  For the flattened index n we first find the row r and column c:
 *      r = floor((sqrt(8*n+1)-1)/2)
 *      c = n - r*(r+1)/2
 *
 *  This implementation computes T(r,c) by dynamic programming,
 *  allocating only a few rows per call.  It works for any
 *  integer type seq_t supplied by the build (including arbitrary‑precise
 *  big integers) and any unsigned idx_t (uint64_t).  No floating point
 *  arithmetic is used; 32‑bit intermediate arithmetic is avoided by
 *  performing operations directly in seq_t.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declarations of helper functions */
static void row_col_from_index(idx_t n, idx_t *r, idx_t *c);
static seq_t compute_T(idx_t r, idx_t c);

/*  Main sequence function  */
seq_t a(idx_t n)
{
    idx_t r, c;
    row_col_from_index(n, &r, &c);
    return compute_T(r, c);
}

/*  Compute the (r,c) entry of the triangle  */
static seq_t compute_T(idx_t r, idx_t c)
{
    /* Base cases */
    if (r == 0)
        return (seq_t)1;
    if (r == 1)
        return (c == 0) ? (seq_t)2 : (seq_t)1;

    /* Allocate three rows of size r+1 each, using explicit cast
       to satisfy C++17. */
    size_t sz = (size_t)(r + 1);
    seq_t *prev2 = (seq_t *)calloc(sz, sizeof(seq_t));   /* row n-2 */
    seq_t *prev1 = (seq_t *)calloc(sz, sizeof(seq_t));   /* row n-1 */
    seq_t *tmp   = (seq_t *)calloc(sz, sizeof(seq_t));   /* current row */

    if (!prev2 || !prev1 || !tmp) {
        free(prev2); free(prev1); free(tmp);
        return (seq_t)0;   /* allocation failure – should never happen */
    }

    /* Initialise the first two rows */
    prev2[0] = (seq_t)1;                            /* row 0 */

    prev1[0] = (seq_t)2;                            /* row 1, column 0 */
    prev1[1] = (seq_t)1;                            /* row 1, column 1 */

    /* Dynamically build rows 2 … r */
    for (idx_t row = 2; row <= r; ++row) {
        for (idx_t k = 0; k <= row; ++k) {
            if (k == row) {          /* diagonal is always 1 */
                tmp[k] = (seq_t)1;
            } else {
                seq_t term1 = ((seq_t)3) * prev1[k];
                seq_t term2 = (k > 0) ? prev1[k-1] : (seq_t)0;
                seq_t term3 = (k <= row-2) ? ((seq_t)3) * prev2[k] : (seq_t)0;
                tmp[k] = term1 + term2 - term3;
            }
        }
        /* Rotate the three row buffers: prev2 <- prev1, prev1 <- tmp, tmp <- prev2 */
        seq_t *swap = prev2;
        prev2 = prev1;
        prev1 = tmp;
        tmp   = swap;
    }

    seq_t result = prev1[c];   /* row r is now in prev1 */

    free(prev2);
    free(prev1);
    free(tmp);
    return result;
}

/*  Determine row r and column c for a given flattened index n  */
static void row_col_from_index(idx_t n, idx_t *r, idx_t *c)
{
    /* Binary search for the largest r such that r*(r+1)/2 ≤ n.
       We use 128‑bit arithmetic to avoid overflow for large n. */
    idx_t low = 0;
    idx_t high = n;  /* r never exceeds n */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }

    *r = low;
    *c = n - (__uint128_t)low * (low + 1) / 2;
}

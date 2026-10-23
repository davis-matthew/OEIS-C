/*
 * A174718  Triangle T(n, k, q) = (1-q^n)*( binomial(n, k) - 1 ) + 1, with q =
 *          2, read by rows.
 * https://oeis.org/A174718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 47, i32 n <= 159, i64 n <= 576,
 *            i128 n <= 2237, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A174718 – Triangle T(n,k,2) read by rows
    Formula:  T(r,k,2) = (1-2^r) * (C(r,k)-1) + 1
    a(n) is the element at index n when the triangle is flattened row‑wise.
    The row number r is the unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2,
    and the column index k = n - r(r+1)/2.  This implementation
    computes r by simple iteration (O(r) ≈ O(√n)) and uses the
    utility functions binom() and pow2() to evaluate the binomial
    coefficient and the power of two. */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */
#include "../utils/binom.h"  /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Determine row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    seq_t tri = 0;               /* triangular number tri(r) = r(r+1)/2 */
    /* Loop until adding the next row would exceed n. */
    while ((tri + (seq_t)(r + 1)) <= (seq_t)n)
    {
        r++;
        tri += (seq_t)r;          /* tri now equals r(r+1)/2 */
    }

    /* Column index within the row */
    idx_t k = (idx_t)((seq_t)n - tri);

    /* Compute 2^r and the binomial coefficient C(r,k). */
    seq_t pow2_r = pow2(r);        /* 2^r */
    seq_t bin     = binom(r, k);   /* C(r,k) */

    /* Apply the triangle formula: (1-2^r)*(C(r,k)-1)+1 */
    seq_t term = (seq_t)1 - pow2_r;
    term = term * (bin - (seq_t)1);
    term += (seq_t)1;

    return term;
}

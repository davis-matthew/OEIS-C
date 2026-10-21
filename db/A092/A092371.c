/*
 * A092371  Triangle read by rows: T(n, k) = binomial(n, k) * binomial(n+k,
 *          n-k).
 * https://oeis.org/A092371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = binomial(r,k) * binomial(r+k, r-k) * where r =
 *            ceil((sqrt(8*n+1)-1)/2) and k = n - r*(r-1)/2 * (row and column
 *            indices are 1‑based). This yields the * OEIS sequence A092371
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _isqrt_seq, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses utility functions:
 *   _isqrt_seq(seq_t)  — floor sqrt of a seq_t value
 *   binomial(idx_t n, idx_t k)  — binomial coefficient */
#include <stdint.h>
#include "../utils/_isqrt_seq.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 1. Find the row r that contains the n-th term.
     *    Compute floor sqrt(8*n+1) and adjust to the smallest r
     *    with r(r+1)/2 ≥ n. */
    idx_t t = 8 * n + 1;                     /* 8*n+1 fits in idx_t for all inputs */
    seq_t sq = _isqrt_seq((seq_t)t);        /* floor sqrt(8*n+1) */
    idx_t r = (idx_t)((sq - 1) / 2);          /* floor of the solution */
    if ((r * (r + 1)) / 2 < n)                /* adjust to the ceil */
        r++;

    /* 2. Find the column k within that row. */
    idx_t prev = (r - 1) * r / 2;           /* terms before row r */
    idx_t k = n - prev;                     /* 1 ≤ k ≤ r */

    /* 3. Compute the triangle entry T(r,k). */
    seq_t left  = binomial(r, k);                  /* C(r, k)   */
    seq_t right = binomial(r + k, r - k);          /* C(r+k, r-k) */

    return left * right;                       /* a(n) = T(r,k) */
}

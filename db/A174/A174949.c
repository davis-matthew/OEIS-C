/*
 * A174949  Triangle read by rows: T(n,k) = B(n,k) - B(n,0) + 1 where B(n,k) =
 *          2 * (5*binomial(n,k)*(n+1)!*k!*(n-k)! + (n+k+1)!*(n-k)! +
 *          (2*n-k+1)!*k!) / ((n+1)!*k!*(n-k)!).
 * https://oeis.org/A174949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) is the linear indexing of the triangle T(r,k) * where *
 *            T(r,k) = 10*binom(r,k) * + 2*binom(r+k+1,k) * +
 *            2*binom(2*r-k+1,r+1) * - 11 * - 2*binom(2*r+1,r+1) * * The row
 *            index r is obtained from the linear index n by * r =
 *            floor((sqrt(8*n+1)-1)/2); k = n - r*(r+1)/2
 * Range:     i8 n <= 7, i16 n <= 28, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to row r and column k. */
    seq_t tn  = (seq_t)n;                 /* cast once */
    seq_t u   = 8*tn + 1;                 /* 8*n + 1   */
    seq_t sq  = floor_sqrt(u);            /* floor(sqrt(8*n+1)) */
    seq_t r_seq = (sq - 1)/2;
    idx_t r = (idx_t)r_seq;               /* row number (zero‑based) */

    /* offset of first element in row r */
    seq_t offs_seq = (seq_t)r*((seq_t)r+1)/2;
    idx_t offs = (idx_t)offs_seq;
    idx_t k = n - offs;                   /* column within the row */

    /* Compute the triangle entry T(r,k).  All intermediate
     * values are seq_t so the computation works for arbitrary
     * precision as well as for fixed width types. */
    seq_t val = 10 * binomial(r, k);

    seq_t c2 = binomial(r + k + 1, k);
    val += 2 * c2;

    seq_t c3 = binomial(2*r - k + 1, r + 1);
    val += 2 * c3;

    seq_t c0 = binomial(2*r + 1, r + 1);
    val -= 2 * c0;

    val -= 11;

    return val;
}

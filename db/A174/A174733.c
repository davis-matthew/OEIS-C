/*
 * A174733  Triangle T(n, k, q) = (1-q^n)*(1/k)*binomial(n-1, k-1)*binomial(n,
 *          k-1) - (1-q^n) + 1, for q = 4, read by rows.
 * https://oeis.org/A174733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   the n-th term a(n) is the flattened triangle T(n,k,4) =
 *            (1-4^n)*(1/k)*binomial(n-1,k-1)*binomial(n,k-1) - (1-4^n) + 1
 *            called with the n-th element of the triangle read by rows. For
 *            A174733 this reduces to T(n,k,4) = (1-4^n)*(A001263(n,k)-1)+1
 *            where A001263(n,k) = (1/k)*C(n-1,k-1)*C(n,k-1)
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 48, i64 n <= 177, i128 n <= 609,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n */
    idx_t r = (idx_t)((sqrt(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    if (r * (r + 1) / 2 < n)
        ++r;

    /* column k in the row r (1-indexed) */
    idx_t k = n - (r - 1) * r / 2;

    /* compute the binomial factors */
    seq_t bin1 = binomial(r - 1, k - 1);
    seq_t bin2 = binomial(r, k - 1);
    seq_t a001263 = (bin1 * bin2) / (seq_t)k;      /* exact division */

    seq_t diff = a001263 - 1;                      /* A001263 - 1 */

    seq_t pow4 = pow_uint(4, r);                   /* 4^r  */

    seq_t temp = (seq_t)1 - pow4;                  /* 1 - 4^r */
    seq_t result = temp * diff + (seq_t)1;          /* final value */

    return result;
}

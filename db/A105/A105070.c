/*
 * A105070  T(n,k) = 2^k*binomial(n,2k+1), where 0 <= k <= floor((n-1)/2), n
 *          >= 1.
 * https://oeis.org/A105070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry of the triangle * T(r,k) = 2^k *
 *            binomial(r,2k+1) (0 ≤ k ≤ ⌊(r−1)/2⌋), * listed row by row. The
 *            index n is first converted to the row r * and column k, then the
 *            value is computed. The routine uses * the helper functions
 *            int_sqrt, binomial and pow2, all of which * return the...
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   binomial, int_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The triangle has ceil(r/2) entries in row r.  We find r and k
     * from the linear index n (1‑based). */
    if (n == 0)               /* out of bounds for this sequence */
        return (seq_t)0;

    idx_t m = int_sqrt(n - 1);        /* floor(sqrt(n‑1))  */
    idx_t t_even = m * (m + 1);      /* n ≤ t_even ? row even else row odd */
    idx_t row, start, k;

    if (n <= t_even) {
        /* Even row r = 2*m.  First index of this row is m*m + 1. */
        row   = 2 * m;
        start = m * m + 1;
        k     = n - start;          /* 0 ≤ k < m */
    } else {
        /* Odd row r = 2*m+1.  First index of this row is m*(m+1)+1. */
        row   = 2 * m + 1;
        start = m * (m + 1) + 1;
        k     = n - start;          /* 0 ≤ k ≤ m */
    }

    /* Compute T(row,k) = 2^k * binomial(row,2k+1). */
    seq_t val = binomial(row, 2 * k + 1);
    val *= pow2(k);
    return val;
}

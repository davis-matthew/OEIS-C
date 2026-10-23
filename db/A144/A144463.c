/*
 * A144463  Triangle T(n,m) read by rows: T(n,m)= A013609(n,m) if m <= n/2,
 *          T(n,m)= T(n,n-m) otherwise.
 * https://oeis.org/A144463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^min(m,r-m) * binomial(r,m) where (r,m) are the row and
 *            column indices of n in the triangle read by rows
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    /* increase high until its offset exceeds n */
    while (((seq_t)high * (high + 1)) / 2 <= n) {
        high <<= 1;
    }
    idx_t r = low;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        seq_t off = ((seq_t)mid * (mid + 1)) / 2;
        if (off <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Column index within the row */
    seq_t row_offset = ((seq_t)r * (r + 1)) / 2;
    idx_t m = n - (idx_t)row_offset;

    /* Compute binomial coefficient C(r, m) */
    seq_t bin = binomial(r, m);

    /* Compute 2^min(m, r-m) */
    idx_t exp = m;
    if (m > r - m)
        exp = r - m;
    seq_t pow = pow2(exp);

    return pow * bin;
}

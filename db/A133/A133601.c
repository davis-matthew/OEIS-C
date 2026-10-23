/*
 * A133601  Triangle read by rows: A007318 * (A097806 + A133080 - I) as
 *          infinite lower triangular matrices, where I is the identity
 *          matrix.
 * https://oeis.org/A133601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the triangle T(r,k) where * T(r,k) = binomial(r,k) +
 *            ((3+(-1)^k)/2) * binomial(r,k+1)
 * Range:     u8 n <= 48, u16 n <= 160, u32 n <= 539, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_n.h"

seq_t a(idx_t n)
{
    /* Find row r and column c such that
     *  r*(r+1)/2 <= n < (r+1)*(r+2)/2   and   c = n - r*(r+1)/2 */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + r + 1 <= n) {
        offset += r + 1;
        ++r;
    }
    idx_t c = n - offset;

    seq_t bin1 = binom_n(r, c);         /* C(r,c)  */
    seq_t bin2 = binom_n(r, c + 1);     /* C(r,c+1) */

    seq_t coeff = (c & 1) ? (seq_t)1 : (seq_t)2; /* (3+(-1)^c)/2 */
    return bin1 + coeff * bin2;
}

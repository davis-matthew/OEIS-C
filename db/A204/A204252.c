/*
 * A204252  Permanent of the n-th principal submatrix of A201208 (in square
 *          format).
 * https://oeis.org/A204252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = o!*e!*Sum_{j=0..e} binomial(o,j)*binomial(e,j)*4^j, with
 *            o = ceiling(n/2), e = floor(n/2); the matrix is M(i,j) = 1 if
 *            i+j even, 2 if i+j odd
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t o = (n + 1) / 2;
    idx_t e = n / 2;
    seq_t sum = 0;
    seq_t p4 = 1;
    for (idx_t j = 0; j <= e; j++) {
        sum += binomial(o, j) * binomial(e, j) * p4;
        p4 *= 4;
    }
    seq_t fo = 1, fe = 1;
    for (idx_t i = 2; i <= o; i++) fo *= (seq_t)i;
    for (idx_t i = 2; i <= e; i++) fe *= (seq_t)i;
    return sum * fo * fe;
}

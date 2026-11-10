/*
 * A204245  Determinant of the n-th principal submatrix of A204244.
 * https://oeis.org/A204245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*a(n-1) - P(n-1), P(n) = n!*P(n-1), with a(1) = 1, P(1)
 *            = 1; equivalently a(n) = P(n)*(1 - Sum_{i=2..n} 1/i!) where P(n)
 *            = Product_{i=2..n} i! (determinant of an arrowhead matrix)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t det = 1, P = 1, f = 1;
    for (idx_t i = 2; i <= n; ++i) {
        f *= (seq_t)i;
        det = f * det - P;
        P = f * P;
    }
    return det;
}

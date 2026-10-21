/*
 * A202098  Number of (n+2) X 8 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(alpha+E,E)*C(alpha+O,O)*C(beta+E,E)*C(beta+O,O) * where
 *            E = 4, O = 4, alpha = floor((n+3)/2), beta = floor((n+2)/2)
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 63, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* out of range (not used) */
    const idx_t E = 4, O = 4;
    idx_t alpha = (n + 3) / 2;      /* floor((n+3)/2) */
    idx_t beta  = (n + 2) / 2;      /* floor((n+2)/2) */

    seq_t v1 = binom(alpha + E, E);
    seq_t v2 = binom(alpha + O, O);
    seq_t v3 = binom(beta  + E, E);
    seq_t v4 = binom(beta  + O, O);

    return v1 * v2 * v3 * v4;
}

/*
 * A096958  Fifth column (m=4) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A096958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+24)*binomial(n+3, 3)/4
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 412, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute (n+24) * C(n+3, 3) / 4 */
    seq_t nn = (seq_t)(n + 24);
    seq_t bin = binomial(n + 3, 3);
    seq_t prod = nn * bin;
    return prod / 4;
}

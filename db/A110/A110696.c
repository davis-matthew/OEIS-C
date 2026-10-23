/*
 * A110696  Kekulé numbers for certain benzenoids of trigonal symmetry.
 * https://oeis.org/A110696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*binomial(2n, n)^3 - 15*binomial(2n, n)^2 +
 *            9*binomial(2n, n) - 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t C   = binomial(2 * n, n);
    seq_t C2  = C * C;
    seq_t C3  = C2 * C;
    seq_t res = C3 * 9;
    res = res - C2 * 15;
    res = res + C * 9;
    res = res - 2;
    return res;
}

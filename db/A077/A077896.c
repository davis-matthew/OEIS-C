/*
 * A077896  Expansion of (1-x)^(-1)/(1+x-2*x^2-2*x^3).
 * https://oeis.org/A077896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (-1)^n)*(2^floor((n + 3)/2) - 1)/2 which simplifies
 *            to a(2k) = 2^(k+1) – 1, a(2k+1) = 0
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 1)
        return 0;
    idx_t k = n >> 1;
    seq_t val = pow2(k + 1);
    val -= 1;
    return val;
}

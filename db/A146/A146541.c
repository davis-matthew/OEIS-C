/*
 * A146541  Binomial transform of A010688.
 * https://oeis.org/A146541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 0); a(n) = 2^(n+2) (n > 0)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    return pow2((idx_t)(n + 2u));
}

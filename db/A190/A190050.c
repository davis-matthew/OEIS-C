/*
 * A190050  Expansion of ((1-x)*(3*x^2-3*x+1))/(1-2*x)^3.
 * https://oeis.org/A190050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (n^2 + 5*n + 10) * 2^(n-4) for n >= 1
 *            (equivalently, a(n) = (n^2 + 5*n + 10) >> (4-n) for n < 4)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sn = (seq_t)n;
    seq_t base = sn * sn + (seq_t)5 * sn + (seq_t)10;

    if (n >= 4)
        return base * pow2(n - 4);
    else
        return base >> (4 - n);
}

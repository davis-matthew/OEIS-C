/*
 * A136962  Numbers k such that k and k^2 use only the digits 0, 5, 6 and 7.
 * https://oeis.org/A136962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 for n = 1 a(n) = 76 * 10^(n-2) for n > 1 // k and k^2
 *            use only digits 0,5,6,7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t t = pow10(n - 2);     /* 10^(n-2) */
    return mul_small(t, 76U);   /* multiply by 76 */
}

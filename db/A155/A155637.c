/*
 * A155637  a(n) = 10^n+5^n-1.
 * https://oeis.org/A155637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 5^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t res = pow10(n);      /* 10^n  */
    res += power5(n);          /* + 5^n */
    res -= 1;                  /* - 1   */
    return res;
}

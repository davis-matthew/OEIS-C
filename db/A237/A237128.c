/*
 * A237128  Angles n expressed in degrees such that 2*cos(n) = phi where phi
 *          is the golden ratio (A001622).
 * https://oeis.org/A237128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18 * (-5 + 3 * (-1)^n + 10 * n)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast n to a signed type for safe arithmetic */
    int64_t nn = (int64_t)n;
    /* (-1)^n : 1 if n even, -1 if n odd */
    int64_t sign = (nn & 1LL) ? -1 : 1;
    /* intermediate calculation */
    int64_t val = 10 * nn + 3 * sign - 5;
    int64_t res = 18 * val;
    return (seq_t)res;
}

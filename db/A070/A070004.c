/*
 * A070004  Numbers of the form 5*2^n or 5*3*2^n; a(n) = 5*A029744(n).
 * https://oeis.org/A070004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n/2) for even n, 15*2^((n-3)/2) for odd n>=3, a(1)=5
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)5;
    }
    if (n & 1u) {
        idx_t exp = (n - 3) / 2;
        return (seq_t)15 * pow2(exp);
    } else {
        idx_t exp = n / 2;
        return (seq_t)5 * pow2(exp);
    }
}

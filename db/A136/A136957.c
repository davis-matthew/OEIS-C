/*
 * A136957  Numbers k such that k and k^2 use only the digits 0, 4, 6 and 9.
 * https://oeis.org/A136957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0, a(n)=64*10^(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }
    return (seq_t)64 * pow10(n - 2);
}

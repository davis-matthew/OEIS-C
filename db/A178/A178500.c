/*
 * A178500  a(n) = 10^n * signum(n).
 * https://oeis.org/A178500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * signum(n) (i.e. a(0)=0, a(n)=10^n for n>0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    return pow10(n);
}

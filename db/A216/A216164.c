/*
 * A216164  Period of powers of 7 mod 10^n.
 * https://oeis.org/A216164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=4, a(2)=4, a(3)=20, a(4)=100, a(n)=5*10^(n-3) for n>=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 4;
    if (n == 3)
        return 20;
    if (n == 4)
        return 100;
    /* n >= 5 */
    return 5 * pow10(n - 3);
}

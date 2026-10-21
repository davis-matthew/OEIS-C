/*
 * A088265  Numbers of the form 10^k + 1, 3, 7, or 9 for k>=1.
 * https://oeis.org/A088265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10^((n-1)/4 + 1) + d_n, where d_n = 1 if n mod 4 == 1, =
 *            3 if n mod 4 == 2, = 7 if n mod 4 == 3, = 9 if n mod 4 == 0.
 *            This generates the numbers 10^k +1,3,7,9 for k≥1, offset 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset 1 */
    idx_t exp = (n - 1) / 4 + 1;           /* exponent of 10 */
    seq_t val = pow10(exp);                /* 10^exp in seq_t */
    idx_t r = n % 4;                       /* remainder modulo 4 */

    switch (r) {
        case 1: val += (seq_t)1; break;
        case 2: val += 3;           break;
        case 3: val += 7;           break;
        case 0: val += 9;           break;
    }
    return val;
}

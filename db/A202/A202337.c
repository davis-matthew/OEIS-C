/*
 * A202337  Range of A062723.
 * https://oeis.org/A202337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=12; a(n)=3*a(n-1) for n>=5
 *            (equivalently a(n)=4*3^(n-3) for n>3)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 12;

    /* n >= 5: return 4 * 3^(n-3) */
    seq_t exp = (seq_t)(n - 3);
    seq_t val = big_pow(3, exp);
    val *= (seq_t)4;
    return val;
}

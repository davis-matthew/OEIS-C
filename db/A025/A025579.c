/*
 * A025579  a(1)=1, a(2)=2, a(n) = 4*3^(n-3) for n >= 3.
 * https://oeis.org/A025579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(n)=4*3^(n-3) for n>=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;          /* n == 1 → 1, n == 0 → 0 (unreached) */
    if (n == 2) return (seq_t)2;
    idx_t exp = n - 3;                    /* exponent for 3 */
    seq_t pow_val = pow3(exp);            /* 3^(n-3) */
    return (seq_t)4 * pow_val;            /* 4 * 3^(n-3) */
}

/*
 * A199684  a(n) = 4*10^n+1.
 * https://oeis.org/A199684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*10^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t val = pow10(n);   /* 10^n */
    val *= (seq_t)4;        /* 4 * 10^n */
    val += (seq_t)1;        /* + 1 */
    return val;
}

/*
 * A177080  a(n) = 5*(10^n-7).
 * https://oeis.org/A177080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(10^n-7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t val = pow10(n);   /* 10^n */
    val = val * 5;          /* 5*10^n */
    return val - 35;        /* 5*10^n - 35 = 5*(10^n - 7) */
}

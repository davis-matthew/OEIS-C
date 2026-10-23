/*
 * A155669  a(n) = 10^n + 9^n - 1.
 * https://oeis.org/A155669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 9^n – 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t p10 = pow10(n);                 /* 10^n          */
    seq_t p9  = pow_int((seq_t)9, n);     /* 9^n           */
    return p10 + p9 - (seq_t)1;           /* 10^n + 9^n – 1*/
}

/*
 * A021311  Decimal expansion of 1/307.
 * https://oeis.org/A021311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^n mod 307) * 10 / 307)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 10^n (mod 307) */
    seq_t pow10 = mod_pow((seq_t)10, n, (seq_t)307);
    /* nth digit after the decimal point of 1/307 */
    return (pow10 * 10) / (seq_t)307;
}

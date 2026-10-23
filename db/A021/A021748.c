/*
 * A021748  Decimal expansion of 1/744.
 * https://oeis.org/A021748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/744) mod 10 (nth digit of the decimal
 *            expansion of 1/744)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t t = mod_pow(10, n + 1, 7440);  /* 10^(n+1) mod 7440 */
    return t / 744;                      /* floor division gives the desired digit (0‑9) */
}

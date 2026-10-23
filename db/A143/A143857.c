/*
 * A143857  a(n) = n + (n+1)*(n+2)^(n+3).
 * https://oeis.org/A143857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n+1)*(n+2)^(n+3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute (n+2)^(n+3) in arbitrary precision */
    seq_t base = (seq_t)n + 2;
    seq_t exp  = (seq_t)n + 3;
    seq_t pow  = big_pow(base, exp);

    /* multiply by (n+1) and add n */
    pow = (seq_t)(n + 1) * pow;
    pow += (seq_t)n;

    return pow;
}

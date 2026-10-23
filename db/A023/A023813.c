/*
 * A023813  a(n) = n^(n*(n+1)/2).
 * https://oeis.org/A023813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0: 0^0 is conventionally 1 for this sequence. */
    if (n == 0)
        return (seq_t)1;

    /* Compute the exponent e = n*(n+1)/2 as a seq_t value. */
    seq_t sn = (seq_t)n;
    seq_t exp = (sn * (sn + 1)) / 2;

    /* Compute n^exp using the provided arbitrary‑precision routine. */
    return big_pow(sn, exp);
}

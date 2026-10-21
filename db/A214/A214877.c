/*
 * A214877  a(n) = n ^ (last digit of n).
 * https://oeis.org/A214877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^(last digit of n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 17, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Handles the 0^0 case by defining a(0) = 1 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 0^0 = 1 by definition for this sequence */
    if (n == 0) {
        return (seq_t)1;
    }

    /* last digit of n (decimal) */
    idx_t d = n % 10;

    /* n^0 = 1, covered explicitly to avoid calling big_pow with exp=0 */
    if (d == 0) {
        return (seq_t)1;
    }

    return big_pow((seq_t)n, (seq_t)d);
}

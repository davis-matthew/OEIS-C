/*
 * A228254  Number of third differences of arrays of length n+3 of numbers in
 *          0..2.
 * https://oeis.org/A228254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n+3) - 2^(n+3) for n>3 Initial values: a(1) = 17, a(2)
 *            = 143, a(3) = 621 This implements OEIS A228254
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 1) return 17;
    if (n == 2) return 143;
    if (n == 3) return 621;

    /* For n > 3: compute 3^(n+3) - 2^(n+3). */
    seq_t exp = (seq_t)n + 3;
    seq_t p3 = big_pow(3, exp);
    seq_t p2 = big_pow(2, exp);
    return p3 - p2;
}

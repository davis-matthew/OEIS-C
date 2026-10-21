/*
 * A059409  a(n) = 4^n * (2^n - 1).
 * https://oeis.org/A059409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (2^n - 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0, so we can return early */
    if (n == 0) {
        return 0;
    }

    /* Compute 2^n and 2^(2n) = 4^n */
    seq_t two = 2;
    seq_t pow2n   = big_pow(two, (seq_t)n);
    seq_t pow2_2n = big_pow(two, (seq_t)(n << 1));

    /* a(n) = 4^n * (2^n - 1) */
    return pow2_2n * (pow2n - 1);
}

/*
 * A124899  Sierpinski quotient ((2n-1)^(2n-1) + 1)/(2n) = A014566(2n-1)/(2n).
 * https://oeis.org/A124899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-1)^(2n-1) + 1)/(2n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* n must be >= 1; if 0 is passed, return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute 2n and 2n-1 as seq_t values. */
    seq_t two_n   = (seq_t)n * 2;
    seq_t base    = (seq_t)2 * (seq_t)n - 1;   /* 2n-1 */
    seq_t exp     = base;                     /* exponent is the same, 2n-1 */

    /* Compute (2n-1)^(2n-1). */
    seq_t pow_val = big_pow(base, exp);

    /* Compute numerator ((2n-1)^(2n-1) + 1). */
    seq_t numerator = pow_val + 1;

    /* Final division (exact). */
    return numerator / two_n;
}

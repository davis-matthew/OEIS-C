/*
 * A013791  a(n) = 9^(4*n + 3).
 * https://oeis.org/A013791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 9^(4*n + 3) = 9^3 * (9^4)^n = 729 * 6561^n */
    const seq_t init = (seq_t)729;
    const seq_t mult = (seq_t)6561;
    seq_t pow = big_pow(mult, (seq_t)n);
    return init * pow;
}

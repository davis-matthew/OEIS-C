/*
 * A139626  a(n) = binomial(n+4, 4)*6^n.
 * https://oeis.org/A139626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4) * 6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient (n+4 choose 4)   */
    seq_t bin = binom(n + 4, 4);

    /* Compute 6^n                                 */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);

    /* Result is the product of the two parts      */
    return bin * pow6;
}

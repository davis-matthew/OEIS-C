/*
 * A199567  a(n) = 7*9^n + 1.
 * https://oeis.org/A199567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*9^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* compute 9^n */
    seq_t power = big_pow((seq_t)9, (seq_t)n);
    /* multiply by 7 */
    seq_t result = mul_small(power, 7u);
    /* add 1 */
    return result + 1;
}

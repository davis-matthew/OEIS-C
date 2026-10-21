/*
 * A078734  Start with 1,2, concatenate 2^k previous terms and change last
 *          term as follows: 1->2, 2->3, 3->1.
 * https://oeis.org/A078734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ((v2(n) mod 3) + 1), where v2(n) is the exponent of 2 in
 *            n
 * Range:     u8 n <= 8823, u16 n <= 8823, u32 n <= 8823, u64 n <= 8823,
 *            u128 n <= 8823, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count the exponent of 2 (number of trailing zeros in n). */
    idx_t e = 0;
    while ((n & 1U) == 0U) {
        e++;
        n >>= 1;
    }
    return (seq_t)((e % 3) + 1);
}

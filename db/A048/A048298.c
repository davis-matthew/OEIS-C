/*
 * A048298  a(n) = n if n=2^i for i >= 0, otherwise a(n) = 0.
 * https://oeis.org/A048298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = n if n = 2^i (i ≥ 0); otherwise a(n) = 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) {
        return 0;
    }

    /* power-of-two test: only one bit set */
    if ((n & (n - 1)) == 0) {
        /* n is a power of two ⇒ return n itself */
        return (seq_t)n;
    }

    /* all other numbers => 0 */
    return 0;
}

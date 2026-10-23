/*
 * A160467  a(n) = 1 if n is odd; otherwise, a(n) = 2^(k-1) where 2^k is the
 *          largest power of 2 that divides n.
 * https://oeis.org/A160467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, mult
 * Formula:   a(n) = 1 if n is odd; otherwise a(n) = 2^(k‑1), * where 2^k is
 *            the largest power of two dividing n
 * Range:     u8 n <= 511, u16 n <= 9139, u32 n <= 9139, u64 n <= 9139,
 *            u128 n <= 9139, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Odd indices produce 1. */
    if ((n & 1) != 0)
        return (seq_t)1;

    /* Count trailing zeros to find the exponent k (k >= 1). */
    idx_t temp = n;
    idx_t k = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++k;
    }

    /* Return 2^(k-1) using a left shift. */
    return (seq_t)((seq_t)1 << (k - 1));
}

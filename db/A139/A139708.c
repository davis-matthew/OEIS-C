/*
 * A139708  Take n in binary. Rotate the binary digits to the left until a 1
 *          once again appears as the leftmost digit. Convert back into
 *          decimal for a(n).
 * https://oeis.org/A139708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy, changed
 * Formula:   a(n) = take the binary representation of n, rotate the bits to
 *            the left until the leading bit is again 1 (at least one rotation
 *            is performed), convert back to decimal
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but defensive */

    seq_t val = (seq_t)n;           /* current rotated value */
    size_t L = bitlen_seq_t(val);   /* fixed bit length of the original n */

    /* create a mask with L lowest bits set */
    size_t width = sizeof(seq_t) * CHAR_BIT;
    seq_t mask;
    if (L == width) {
        mask = ~(seq_t)0;           /* all bits 1 */
    } else {
        mask = ((((seq_t)1) << L) - 1);
    }

    /* perform at least one rotation */
    val = (((val << 1) | (val >> (L - 1))) & mask);

    /* keep rotating until the leading bit becomes 1 */
    while ((val & (((seq_t)1) << (L - 1))) == 0) {
        val = (((val << 1) | (val >> (L - 1))) & mask);
    }

    return val;
}

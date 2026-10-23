/*
 * A175665  The product of maximal run and minimal run lengths in the binary
 *          representation of n.
 * https://oeis.org/A175665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A043276(n) * A175597(n) product of maximal run and
 *            minimal run lengths in the binary representation of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t (unused) */

seq_t a(idx_t n)
{
    /* Handle offset 1: a(1) corresponds to n = 1 */
    if (n == 0) return (seq_t)0;

    /* Find the position of the most significant bit (0‑based). */
    idx_t msb = 0;
    idx_t temp = n;
    while (temp >>= 1) {
        ++msb;
    }

    /* Initialize run tracking. */
    idx_t current_bit = (n >> msb) & 1;
    idx_t count = 1;
    idx_t max_len = 1;
    idx_t min_len = (idx_t)-1;          /* large value */

    /* Process remaining bits from msb‑1 down to 0. */
    for (idx_t i = msb; i > 0; --i) {
        idx_t bit = (n >> (i - 1)) & 1;
        if (bit == current_bit) {
            ++count;
        } else {
            if (count > max_len) max_len = count;
            if (count < min_len) min_len = count;
            current_bit = bit;
            count = 1;
        }
    }

    /* Update with the final run. */
    if (count > max_len) max_len = count;
    if (count < min_len) min_len = count;

    /* Return the product in seq_t type. */
    return (seq_t)(max_len * min_len);
}

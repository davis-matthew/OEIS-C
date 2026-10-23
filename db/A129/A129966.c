/*
 * A129966  Triangular numbers which are differences of squares.
 * https://oeis.org/A129966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(m) where m is the n-th non‑negative integer that is *
 *            NOT congruent to 3 or 4 modulo 8. * Here T(x) = x*(x+1)/2
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special handling for indices below the offset (not used). */
    if (n == 0) return 0;

    /* 0‑based index in the filtered sequence */
    idx_t k0 = n - 1;

    /* Each block of 8 integers contains 6 admissible ones:
     * 0,1,2,5,6,7 (mod 8)                               */
    const idx_t offset[6] = {0, 1, 2, 5, 6, 7};

    idx_t full_block = k0 / 6;
    idx_t rem        = k0 % 6;

    /* The actual integer m whose triangular number is a(n) */
    seq_t m = (seq_t)(full_block * 8 + offset[rem]);

    /* Compute T(m) safely: divide one operand by 2 before
     * multiplication to avoid overflow when seq_t is a
     * fixed-width unsigned integer. */
    if ((m & (seq_t)1) == 0) {
        return (m / (seq_t)2) * (m + (seq_t)1);
    } else {
        return m * ((m + (seq_t)1) / (seq_t)2);
    }
}

/*
 * A145807  1/2 the number of islands of ones fitting in an n X n array
 *          symmetric under 180 degree rotation with all ones connected only
 *          either two adjacent vertically or two adjacent horizontally.
 * https://oeis.org/A145807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n²/8⌋ for n ≤ 8; a(n) = ⌊n²/8⌋ – 1 for n > 8 and n mod 8
 *            ∉ {0,2,4}
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 8) {
        /* For small n the formula reduces to ⌊n²/8⌋. */
        return ((seq_t)n * (seq_t)n) / 8;
    }

    /* n > 8 */
    idx_t mod = n & 7U;  /* n % 8 */
    seq_t res = ((seq_t)n * (seq_t)n) / 8;

    if (mod != 0 && mod != 2 && mod != 4) {
        /* Subtract one for the remaining congruence classes. */
        res -= 1;
    }
    return res;
}

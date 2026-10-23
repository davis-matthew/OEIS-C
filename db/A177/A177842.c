/*
 * A177842  Period 27: repeat [1, 81, 81, 3, 81, 81, 9, 81, 81, 3, 81, 81, 3,
 *          81, 81, 9, 81, 81, 3, 81, 81, 1, 81, 81, 9, 81, 81].
 * https://oeis.org/A177842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A061040(n+3) - 9*A061039(n+3)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* periodic pattern of length 27 */
    static const seq_t period[27] = {
        1, 81, 81, 3, 81, 81, 9,
        81, 81, 3, 81, 81, 3, 81,
        81, 9, 81, 81, 3, 81, 81,
        1, 81, 81, 9, 81, 81
    };

    return period[(size_t)(n % 27ULL)];
}

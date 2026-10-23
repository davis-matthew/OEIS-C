/*
 * A131531  Period 6: repeat [0, 0, 1, 0, 0, -1].
 * https://oeis.org/A131531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 92, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period 6 pattern: [0, 0, 1, 0, 0, -1] with offset 1 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t r = n % 6;
    static const seq_t pattern[6] = {
        (seq_t)-1, /* r == 0  → a(n) = -1 */
        (seq_t)0,  /* r == 1  → a(n) = 0  */
        (seq_t)0,  /* r == 2  → a(n) = 0  */
        (seq_t)1,  /* r == 3  → a(n) = 1  */
        (seq_t)0,  /* r == 4  → a(n) = 0  */
        (seq_t)0   /* r == 5  → a(n) = 0  */
    };
    return pattern[(size_t)r];
}

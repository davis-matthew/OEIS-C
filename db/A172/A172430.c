/*
 * A172430  Period length 10: repeat [2, 1, 6, 7, 0, 3, 4, 9, 8, 5].
 * https://oeis.org/A172430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = [2, 1, 6, 7, 0, 3, 4, 9, 8, 5][n mod 10]
 * The sequence is periodic with period length 10.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* period pattern */
    static const seq_t pat[10] = {2, 1, 6, 7, 0, 3, 4, 9, 8, 5};
    return pat[(size_t)(n % 10)];
}

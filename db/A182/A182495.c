/*
 * A182495  Number of letters in Turkish words for months of year.
 * https://oeis.org/A182495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = a(n-12) for n > 12; a(1)=4, a(2)=5, a(3)=4, a(4)=5,
 *            a(5)=5, a(6)=7, a(7)=6, a(8)=7, a(9)=5, a(10)=4, a(11)=5,
 *            a(12)=6
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* (n-1) modulo 12 gives position in the period, n >= 1 */
    idx_t r = (n + 11) % 12;  /* equivalent to (n-1) % 12 */

    static const seq_t period[12] = {
        4, 5, 4, 5, 5, 7,
        6, 7, 5, 4, 5, 6
    };

    return period[(size_t)r];
}

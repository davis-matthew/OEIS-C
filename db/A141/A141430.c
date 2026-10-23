/*
 * A141430  a(n) = A000111(n) mod 9.
 * https://oeis.org/A141430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A000111(n) mod 9, periodic with period 12 after n>=2
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0), a(1) and a(2) are all 1 */
    if (n <= 2) return 1;

    /* Periodic part (indices 2..13 inclusive, then repeats every 12) */
    static const seq_t period[12] = {
        1, /* a(2) */
        2, /* a(3) */
        5, /* a(4) */
        7, /* a(5) */
        7, /* a(6) */
        2, /* a(7) */
        8, /* a(8) */
        7, /* a(9) */
        4, /* a(10) */
        2, /* a(11) */
        2, /* a(12) */
        7  /* a(13) */
    };

    idx_t idx = (n - 2) % 12;          /* 0 <= idx < 12 */
    return period[(size_t)idx];
}

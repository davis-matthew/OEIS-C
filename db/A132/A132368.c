/*
 * A132368  a(0), ..., a(7) are 0,1,2,3,6,4,5,7; for n >= 8, a(n) = a(n-8) +
 *          8.
 * https://oeis.org/A132368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-8) + 8  with initial values
 * a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3,
 * a(4) = 6, a(5) = 4, a(6) = 5, a(7) = 7.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Value for the first eight indices */
    static const seq_t init[8] = {0, 1, 2, 3, 6, 4, 5, 7};

    /* n mod 8 */
    size_t ridx = (size_t)(n & 7);
    seq_t   r    = init[ridx];      /* a(n mod 8) */

    /* floor(n/8) as seq_t */
    seq_t   q    = (seq_t)(n >> 3);  /* truncate to the width of seq_t */

    return (q << 3) + r;             /* 8 * floor(n/8) + a(n mod 8) */
}

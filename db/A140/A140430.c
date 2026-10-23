/*
 * A140430  Period 6: repeat [3, 2, 4, 1, 2, 0].
 * https://oeis.org/A140430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)-a(n-3)+a(n-4), a(0)=3, a(1)=2, a(2)=4, a(3)=1,
 *            a(4)=2, a(5)=0
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, a(n) repeats the period [3, 2, 4, 1, 2, 0] for n ≥ 0) */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic sequence with period 6: 3, 2, 4, 1, 2, 0. */
    static const seq_t period[6] = { 3, 2, 4, 1, 2, 0 };
    return period[(size_t)(n % 6)];
}

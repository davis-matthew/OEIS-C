/*
 * A099216  a(n)=4a(n-1)-4a(n-2)+2a(n-3).
 * https://oeis.org/A099216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3), with initial values
 *            a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Directly return the known small values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* Use three sliding variables to keep the recurrence O(1) memory. */
    seq_t a_nm3 = (seq_t)1;  /* a(n-3) for current i */
    seq_t a_nm2 = (seq_t)2;  /* a(n-2) for current i */
    seq_t a_nm1 = (seq_t)4;  /* a(n-1) for current i */
    seq_t a_n   = 0;        /* a(n) placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 4 * a_nm1;         /* 4*a(n-1) */
        a_n -= 4 * a_nm2;        /* -4*a(n-2) */
        a_n += 2 * a_nm3;        /* +2*a(n-3) */

        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

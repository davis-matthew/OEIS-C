/*
 * A206839  Number of 1 X n 0..3 arrays avoiding the pattern z-2 z-1 z in any
 *          row, column, nw-to-se diagonal or ne-to-sw antidiagonal.
 * https://oeis.org/A206839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-3) + a(n-4) Initial values: a(0)=1,
 *            a(1)=4, a(2)=16, a(3)=62
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Explicitly return the first four terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)62;

    seq_t a0 = (seq_t)1;  /* a(n-4)   */
    seq_t a1 = (seq_t)4;  /* a(n-3)   */
    seq_t a2 = (seq_t)16; /* a(n-2)   */
    seq_t a3 = (seq_t)62; /* a(n-1)   */

    const seq_t four = (seq_t)4;
    const seq_t two  = (seq_t)2;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 4*a(i-1) - 2*a(i-3) + a(i-4) */
        seq_t next = four * a3 - two * a1 + a0;
        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;   /* a(n) */
}

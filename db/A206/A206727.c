/*
 * A206727  Number of nX1 0..2 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-3) + a(n-5) Initial values: a(1)=3,
 *            a(2)=9, a(3)=25, a(4)=69, a(5)=190
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the recurrence iteratively, using only a
   fixed buffer of the last five terms; no dynamic allocation is required,
   and it works in C11 or C++17 for any integer type seq_t. */

#include <stdint.h> /* For portable integer types if needed */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)69;
    if (n == 5) return (seq_t)190;

    /* Buffer holds a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t buf[5] = { (seq_t)3, (seq_t)9, (seq_t)25, (seq_t)69, (seq_t)190 };

    for (idx_t i = 6; i <= n; ++i)
    {
        /* Compute next term using the recurrence with the buffer entries */
        seq_t next = 3 * buf[4] + buf[0];   /* 3*a(n-1) + a(n-5) */
        next -= 2 * buf[2];                 /* subtract 2*a(n-3) */

        /* Shift the buffer: drop a(n-5), shift others left, insert next */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4]; /* after loop, buf[4] holds a(n) */
}

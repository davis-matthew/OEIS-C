/*
 * A217069  a(n) = 3*a(n-1) + 24*a(n-2) + a(n-3), with a(0)=0, a(1)=2, and
 *          a(2)=7.
 * https://oeis.org/A217069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 24*a(n-2) + a(n-3) Initial values: a(0) = 0,
 *            a(1) = 2, a(2) = 7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

/* The function computes the nth term of sequence A217069
   using an iterative recurrence that works for any seq_t
   (built‑in integer or arbitrary‑precision integer). */
seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;

    /* Keep the last three computed values. */
    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)7;   /* a(n-1) */
    seq_t a3;              /* a(n) for the current step */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 3 * a2 + 24 * a1 + a0;  /* recurrence */
        a0 = a1;                     /* shift window */
        a1 = a2;
        a2 = a3;
    }

    return a2;   /* a(n) after the loop */
}

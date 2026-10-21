/*
 * A220547  Number of ways to reciprocally link elements of an n X 2 array
 *          either to themselves or to exactly one horizontal, vertical or
 *          antidiagonal neighbor.
 * https://oeis.org/A220547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4)
   Base values: a(1)=2, a(2)=8, a(3)=29, a(4)=108. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* safety – undefined for n=0, but return 0 */
    if (n == 0) return (seq_t)0;

    /* base cases directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)29;
    if (n == 4) return (seq_t)108;

    /* compute iteratively using only the previous 4 terms */
    seq_t a1 = (seq_t)2;   /* a(n-4) */
    seq_t a2 = (seq_t)8;   /* a(n-3) */
    seq_t a3 = (seq_t)29;  /* a(n-2) */
    seq_t a4 = (seq_t)108; /* a(n-1) */

    seq_t next;
    idx_t i;
    for (i = 5; i <= n; ++i) {
        next = 3*a4 + 3*a3 - a2 - a1;  /* a(n) */
        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}

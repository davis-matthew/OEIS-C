/*
 * A017065  Expansion of 1/((1-3x)(1-4x)(1-11x)).
 * https://oeis.org/A017065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 89*a(n-2) + 132*a(n-3) Base values: a(0)=1,
 *            a(1)=18, a(2)=235 Uses an O(n) iterative recurrence – no cache
 *            or dynamic allocation needed
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)235;

    /* a0, a1, a2 hold a(n-3), a(n-2), a(n-1) respectively. */
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)18;  /* a(1)   */
    seq_t a2 = (seq_t)235; /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* 18*a(n-1) + 132*a(n-3) - 89*a(n-2)  (addition first to avoid
           unsigned underflow, for the 64‑bit build). */
        cur = (seq_t)18 * a2 + (seq_t)132 * a0 - (seq_t)89 * a1;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }

    return a2;
}

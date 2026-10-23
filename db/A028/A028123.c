/*
 * A028123  Expansion of 1/((1-4x)(1-5x)(1-8x)(1-12x)).
 * https://oeis.org/A028123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 29*a(n-1) – 296*a(n-2) + 1264*a(n-3) – 1920*a(n-4),  n ≥ 4
 * Initial values: a(0) = 1, a(1) = 29, a(2) = 545, a(3) = 8485
 *
 * This implementation computes the term by iterating the linear
 * recurrence from 0 up to n, which works for any seq_t type
 * (including arbitrary‑precision classes), satisfies the O(n) time
 * requirement and does not rely on dynamic memory allocation.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)545;
    if (n == 3) return (seq_t)8485;

    /* Iteratively apply the recurrence */
    seq_t a0 = (seq_t)1;    /* a(n-4) */
    seq_t a1 = (seq_t)29;   /* a(n-3) */
    seq_t a2 = (seq_t)545;  /* a(n-2) */
    seq_t a3 = (seq_t)8485; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = 29 * a3 - 296 * a2 + 1264 * a1 - 1920 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}

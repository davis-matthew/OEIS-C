/*
 * A021384  Expansion of 1/((1-x)(1-3x)(1-4x)(1-10x)).
 * https://oeis.org/A021384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=18, a(2)=225, a(3)=2470, for n≥4:
 *            a(n)=18*a(n-1)-99*a(n-2)+202*a(n-3)-120*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the 4‑term linear recurrence. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)225;
    if (n == 3) return (seq_t)2470;

    /* t0..t3 will hold a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t t0 = (seq_t)1;     /* a(0)   */
    seq_t t1 = (seq_t)18;    /* a(1)   */
    seq_t t2 = (seq_t)225;   /* a(2)   */
    seq_t t3 = (seq_t)2470;  /* a(3)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute 18*t3 + 202*t1 - 99*t2 - 120*t0,
           using unsigned arithmetic only (t3+202*t1 >= 99*t2+120*t0). */
        next = 18 * t3;
        next += 202 * t1;
        next -= 99 * t2;
        next -= 120 * t0;

        /* Shift the window. */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}

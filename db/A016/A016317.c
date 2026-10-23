/*
 * A016317  Expansion of 1/((1-2x)(1-8x)(1-10x)).
 * https://oeis.org/A016317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016317, Expansion of 1/((1-2x)(1-8x)(1-10x))
   Formula: a(n) = 20*a(n-1) - 116*a(n-2) + 160*a(n-3)  for n ≥ 3
   Initial values: a(0)=1, a(1)=20, a(2)=284 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)284;

    /* Three previous terms needed for the recurrence. */
    seq_t a0 = (seq_t)1;      /* a(n-3) */
    seq_t a1 = (seq_t)20;     /* a(n-2) */
    seq_t a2 = (seq_t)284;    /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)20 * a2   /* 20*a(n-1) */
           - (seq_t)116 * a1  /* 116*a(n-2) */
           + (seq_t)160 * a0; /* 160*a(n-3) */
        /* Slide the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}

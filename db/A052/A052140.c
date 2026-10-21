/*
 * A052140  a(n) = 4^n * n!^2 * Sum_{k=0..n} 1/k!.
 * https://oeis.org/A052140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2*a(n-1) + 4^n * n!, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function must use only integer arithmetic (no <math.h>).
 * seq_t and idx_t are defined by the build system; do not re‑define them. */

seq_t a(idx_t n)
{
    seq_t prev = 1;          /* a(0) */
    seq_t fact = 1;          /* factorial accumulates: (i-1)!  */
    seq_t pow4 = 1;          /* 4^(i-1)                               */

    /* Iterate from n=1 up to the requested n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* update factorial and 4^i */
        fact = fact * (seq_t)i;
        pow4 = pow4 * 4;

        /* term1 = 4 * i^2 * a(i-1)  */
        seq_t i_sq = (seq_t)i * (seq_t)i;
        seq_t term1 = 4 * i_sq * prev;

        /* term2 = 4^i * i!           */
        seq_t term2 = pow4 * fact;

        /* new value a(i) */
        prev = term1 + term2;
    }

    return prev;
}

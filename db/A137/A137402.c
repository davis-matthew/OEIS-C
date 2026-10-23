/*
 * A137402  a(n) = Sum_{k=0..n} binomial(floor(n-2k/3), k).
 * https://oeis.org/A137402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 84, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137402
   Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5)
   Initial terms: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5 */

#include <stddef.h>   /* for size_t if needed */

/* Compute a(n) for any n >= 0.  The result is in the same type as seq_t. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;

    /* Sliding window: prev[0] = a(i-5), prev[1] = a(i-4), …, prev[4] = a(i-1) */
    seq_t prev[5] = {
        (seq_t)1,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)2,  /* a(2) */
        (seq_t)3,  /* a(3) */
        (seq_t)5   /* a(4) */
    };

    seq_t term = 0;
    for (idx_t i = 5; i <= n; ++i) {
        /* Compute using arithmetic only */
        term = prev[4] * (seq_t)3;      /* 3*a(i-1) */
        term -= prev[3] * (seq_t)3;      /* -3*a(i-2) */
        term += prev[2];                 /* +a(i-3) */
        term += prev[0];                 /* +a(i-5) */

        /* Shift the window: drop oldest, add newest */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = term;
    }
    return term;
}

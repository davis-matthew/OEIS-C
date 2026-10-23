/*
 * A048907  Indices of 9-gonal numbers which are also triangular.
 * https://oeis.org/A048907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048907: Indices of 9‑gonal numbers which are also triangular. */
/* Recurrence: a(1)=1, a(2)=10,  a(n+2)=16*a(n+1)-a(n)-5  for n≥1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offsets explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;
    if (n == 0) return (seq_t)0;   /* Undefined for n<1, return 0. */

    /* Iterative computation using the linear recurrence. */
    seq_t a_prev2 = (seq_t)1;       /* a(1) */
    seq_t a_prev1 = (seq_t)10;      /* a(2) */
    seq_t a_curr   = a_prev1;       /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = a_prev1 * (seq_t)16;   /* 16*a(n+1) */
        a_curr -= a_prev2;              /* -a(n)   */
        a_curr -= (seq_t)5;             /* -5      */

        a_prev2 = a_prev1;              /* shift window */
        a_prev1 = a_curr;
    }
    return a_curr;
}

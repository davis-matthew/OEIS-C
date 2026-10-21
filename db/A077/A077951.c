/*
 * A077951  Expansion of 1/(1-x+x^2-2*x^3).
 * https://oeis.org/A077951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 75, u64 n <= 149,
 *            u128 n <= 295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n)  from the generating function 1/(1 - x + x^2 - 2x^3)
   Recurrence:
     a(0) = 1
     a(1) = 1
     a(2) = 0
     a(n) = a(n-1) + 2*a(n-3) - a(n-2)   for n ≥ 3
   (same as a(n) = a(n-1) - a(n-2) + 2*a(n-3)). */
seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    /* a0 = a(0), a1 = a(1), a2 = a(2) */
    seq_t a_prev3 = (seq_t)1;      /* a(n-3) */
    seq_t a_prev2 = (seq_t)1;      /* a(n-2) */
    seq_t a_prev1 = (seq_t)0;      /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute a(i) = a_prev1 + 2*a_prev3 - a_prev2 */
        seq_t temp = a_prev3 + a_prev3;          /* 2*a_prev3 */
        seq_t cur   = a_prev1 + temp - a_prev2;

        /* Shift the window for the next iteration. */
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}

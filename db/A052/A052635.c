/*
 * A052635  E.g.f. (1-3x)/(1-3x-x^2).
 * https://oeis.org/A052635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A052635):
   a(0) = 1, a(1) = 0,
   a(n+2) = (n^2 + 3*n + 2) * a(n) + (3*n + 6) * a(n+1)   (n ≥ 0) */

/* Computes a(n) for any non‑negative n.
   The function is O(n) in time and O(1) in space, and works for
   all supported seq_t types (fixed‑size or arbitrary precision). */
seq_t a(idx_t n)
{
    /* Special base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* Keep the two most recent values only. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)0;   /* a(1) */
    seq_t a_curr;

    /* Iterate from 2 up to n using the recurrence. */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t m      = (seq_t)(i - 2);                 /* value of n in the formula */
        seq_t coeffA = (seq_t)2 + m * m + (seq_t)3 * m; /* n^2 + 3n + 2 */
        seq_t coeffB = (seq_t)6 + (seq_t)3 * m;         /* 3n + 6 */

        a_curr = coeffA * a_prev2 + coeffB * a_prev1;

        /* Slide the window for the next iteration. */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;   /* a_prev1 holds a(n) after the loop */
}

/*
 * A049663  a(n) = (F(6*n+5) - 1)/4, where F = A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(6*n+5) - 1)/4 Recurrence used: a(0)=1, a(1)=22,
 *            a(2)=399, a(n) = 19*a(n-1) - 19*a(n-2) + a(n-3) (n ≥ 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)399;

    /* Iteratively compute the recurrence.
       prev3 = a(i-3), prev2 = a(i-2), prev1 = a(i-1). */
    seq_t prev3 = (seq_t)1;    /* a(0) */
    seq_t prev2 = (seq_t)22;   /* a(1) */
    seq_t prev1 = (seq_t)399;  /* a(2) */
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Dual multiplication by 19 is done via cast to seq_t
           to avoid type mismatch for both integer and big‑int builds. */
        seq_t t1 = (seq_t)19 * prev1;
        seq_t t2 = (seq_t)19 * prev2;
        curr = t1 - t2 + prev3;

        /* Shift the sliding window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

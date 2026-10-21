/*
 * A085480  Expansion of 3*x*(1+2*x)/(1-3*x-3*x^2).
 * https://oeis.org/A085480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A085480
   Formula: a(n) = 3*a(n-1) + 3*a(n-2)   for n ≥ 4
   Initial values: a(1) = 0, a(2) = 3, a(3) = 15
   (The sequence starts with a(1)=0; the generating function
   is 3*x*(1+2*x)/(1-3*x-3*x^2).  */

seq_t a(idx_t n)
{
    /* Values below the offset are irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Handle the first three terms explicitly. */
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)15;

    /* Use the recurrence a(n) = 3*a(n-1) + 3*a(n-2) for n ≥ 4. */
    seq_t prev2 = (seq_t)3;   /* a(2) */
    seq_t prev1 = (seq_t)15;  /* a(3) */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i)
    {
        curr = (seq_t)3 * prev1 + (seq_t)3 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

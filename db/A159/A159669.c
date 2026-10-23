/*
 * A159669  Expansion of x*(1 + x)/(1 - 28*x + x^2).
 * https://oeis.org/A159669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 28*a(n-1) – a(n-2)
 * with initial values a(1) = 1, a(2) = 29.
 * Offset: 1   (a(1) is the first term in the OEIS entry)
 *
 * The function computes the nth term of the sequence defined by
 * the generating function x*(1+x)/(1-28*x+x^2).
 */
seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; never used in tests. */
    if (n == 0)
        return (seq_t)0;

    /* Handle the first two terms directly. */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)29;

    /* Iterative recurrence using only the two previous values. */
    seq_t a_prev2 = (seq_t)1;     /* a(1) */
    seq_t a_prev1 = (seq_t)29;    /* a(2) */
    seq_t a_cur = a_prev1;        /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_cur = (seq_t)28 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur; /* a(n) for n >= 3 */
}

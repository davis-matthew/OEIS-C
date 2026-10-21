/*
 * A067318  Sum of the reflection lengths of all permutations of n letters.
 * https://oeis.org/A067318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, changed
 * Formula:   a(n) = n * a(n-1) + (n-1) * (n-1)!, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (n >= 1).  For n <= 1 the value is 0. */
    if (n <= 1)
        return (seq_t)0;

    seq_t fact_prev = 1;    /* (i-1)! for the current i in the loop */
    seq_t a_val    = 0;     /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute a(i) = i*a(i-1) + (i-1)*(i-1)! (fact_prev) */
        seq_t next = (seq_t)i * a_val + (i - 1) * fact_prev;

        /* Update factorial: fact_prev becomes i! for the next iteration */
        fact_prev *= (seq_t)i;

        a_val = next;
    }

    return a_val;
}

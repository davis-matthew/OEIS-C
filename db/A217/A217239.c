/*
 * A217239  a(n) = n!*(!n - 1) = n! * Sum_{k=1..n-1} k!.
 * https://oeis.org/A217239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(!n - 1) = n!*Sum_{k=1..n-1} k!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1; /* (n-1)! after the loop */
    seq_t sum  = 0; /* Sum_{k=1}^{n-1} k!  */
    for (idx_t i = 1; i < n; ++i)
    {
        fact *= (seq_t)i; /* factorial increment */
        sum  += fact;    /* add current factorial to sum */
    }
    seq_t nfact = fact * (seq_t)n; /* n!  */
    return nfact * sum;            /* n! * Sum_{k=1}^{n-1} k!  */
}

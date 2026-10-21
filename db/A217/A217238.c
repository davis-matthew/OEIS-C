/*
 * A217238  a(n) = n! * Sum_{k=1..n} k!.
 * https://oeis.org/A217238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=1..n} k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    seq_t fact = (seq_t)1;   /* current factorial i!   */
    seq_t sum  = (seq_t)0;   /* sum of factorials 1! … i! */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        fact *= (seq_t)i;    /* compute i!          */
        sum  += fact;        /* add it to the sum   */
    }
    return fact * sum;       /* n! * (1!+2!+…+n!)   */
}

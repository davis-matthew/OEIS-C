/*
 * A119360  a(n) = Sum_{k=1..n} (k! mod n).
 * https://oeis.org/A119360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k! mod n)
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sum of factorials modulo n, indexed from 1 */
    seq_t sum = 0;
    seq_t fact = 1 % (seq_t)n;      /* initial factorial mod n */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Multiply and reduce modulo n.  Use a wider type to avoid overflow. */
        fact = ((unsigned long long)fact * k) % (seq_t)n;
        sum += fact;
    }

    return sum;
}

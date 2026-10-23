/*
 * A034911  One fifth of octo-factorial numbers.
 * https://oeis.org/A034911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/5) * Π_{j=1..n} (8*j - 3) Recurrence: a(n) = (8*n - 3)
 *            * a(n-1), with a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 as a safe default. */
    if (n == 0)
        return 0;

    seq_t product = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* term = 8*i - 3 ; computed in seq_t to avoid overflow. */
        seq_t term = (seq_t)8 * (seq_t)i - (seq_t)3;
        product *= term;
    }
    /* The product is always divisible by 5. */
    product /= (seq_t)5;
    return product;
}

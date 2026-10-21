/*
 * A052255  Partial sums of A050484.
 * https://oeis.org/A052255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*n+8)*binomial(n+7,7)/8
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 45, u64 n <= 784, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 prod = 1;

    /* Compute binomial(n+7,7) = (n+1)(n+2)…(n+7) / 7! */
    for (idx_t i = 1; i <= 7; ++i)
        prod *= (unsigned __int128)(n + i);
    prod /= 5040;          /* 7! = 5040 */

    /* Multiply by (5*n + 8) and divide by 8 */
    prod *= (unsigned __int128)(5 * (unsigned __int128)n + 8);
    prod /= 8;

    return (seq_t)prod;
}

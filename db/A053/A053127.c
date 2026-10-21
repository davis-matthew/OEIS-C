/*
 * A053127  Binomial coefficients C(2*n-4,5).
 * https://oeis.org/A053127
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-4,5) for n >= 5, a(n) = 0 for n < 5
 * Range:     u8 none, u16 n <= 7, u32 n <= 46, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;

    /* Compute m = 2*n - 4 in seq_t to avoid overflow of idx_t. */
    seq_t m = ((seq_t)n << 1) - (seq_t)4;

    /* Compute product = (m)*(m-1)*(m-2)*(m-3)*(m-4). */
    seq_t product = (seq_t)1;
    for (int i = 0; i < 5; ++i)
        product *= (m - (seq_t)i);

    /* Divide by 5! = 120 to obtain the binomial coefficient. */
    return product / (seq_t)120;
}

/*
 * A060008  a(n) = 9*binomial(n,4) = 3*n*(n-1)*(n-2)*(n-3)/8.
 * https://oeis.org/A060008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*(n-1)*(n-2)*(n-3)/8
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 299, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Compute the product of four consecutive integers as seq_t to avoid overflow */
    seq_t prod = (seq_t)n;
    prod *= (seq_t)(n - 1);
    prod *= (seq_t)(n - 2);
    prod *= (seq_t)(n - 3);

    /* The product is always divisible by 8 (since it is 24 * (n choose 4)) */
    prod /= 8;
    prod *= 3;
    return prod;
}

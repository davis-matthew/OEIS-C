/*
 * A068605  Number of functions from [1,2,...,n] to [1,2,...,n] such that the
 *          image contains exactly two elements.
 * https://oeis.org/A068605
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,2) * (2^n - 2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. For n < 2 we return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Compute C(n,2) = n*(n-1)/2 using seq_t intermediate. */
    seq_t bin = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;

    /* Compute 2^n - 2 by repeated multiplication. */
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        power *= (seq_t)2;
    power -= (seq_t)2;

    /* Return the product. */
    return bin * power;
}

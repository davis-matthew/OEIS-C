/*
 * A066300  Number of n X n matrices with exactly 2 1's in each row, other
 *          entries 0.
 * https://oeis.org/A066300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,2)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n,2) = n*(n-1)/2 */
    seq_t binom = (seq_t)n * ((seq_t)n - 1) / 2;

    /* compute binom^n by successive multiplication */
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= binom;

    return result;
}

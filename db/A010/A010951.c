/*
 * A010951  Binomial coefficient C(35,n).
 * https://oeis.org/A010951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(35,n) (binomial coefficient), 0 for n>35
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the 35th row */
    if (n > 35U) return 0U;

    /* C(35,0) = 1, and the multiplicative formula works for n==0 */
    seq_t result = 1U;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Multiply by current numerator, divide by i.
           All operations are in seq_t to avoid overflow on types up to 128 bits */
        result = (result * ((seq_t)35U - i + 1U)) / (seq_t)i;
    }
    return result;
}

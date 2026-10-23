/*
 * A010941  Binomial coefficient C(25,n).
 * https://oeis.org/A010941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(25,n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 25 the binomial coefficient is defined as 0. */
    if (n > 25) return 0;

    /* Compute C(25,n) using the multiplicative formula, choosing the
       smaller of n and 25-n to minimise work. */
    seq_t result = 1;
    idx_t k = n;
    if (k > 25 - k) k = 25 - k;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* Multiply by the appropriate factor and divide by i.
           All operations stay within the range of seq_t. */
        result = (result * ( (seq_t)(25 - k + i) )) / (seq_t)i;
    }

    return result;
}

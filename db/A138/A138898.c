/*
 * A138898  Ratio of (2*n-1)! to number of zeros in lower part of Sylvester
 *          matrix for polynomial of degree n with all nonzero coefficients.
 * https://oeis.org/A138898
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)!/((n-1)*(n-2))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3 */
    if (n < 3)
        return (seq_t)0;

    /* Compute (2*n-1)! while skipping the factors (n-1) and (n-2). */
    idx_t limit = 2 * n - 1;
    seq_t prod = (seq_t)1;

    for (idx_t k = 1; k <= limit; ++k)
    {
        if (k == n - 1 || k == n - 2)
            continue;             /* skip division factors */
        prod *= (seq_t)k;
    }

    return prod;
}

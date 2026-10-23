/*
 * A010929  Binomial coefficient C(13,n).
 * https://oeis.org/A010929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(13, n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the binomial coefficient C(13, n).
   For n > 13, the binomial coefficient is 0. */
seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the 13th row */
    if (n > (idx_t)13)
        return 0;

    /* Use symmetry to reduce the number of iterations */
    idx_t k = n;
    idx_t max = (idx_t)13;
    if (k > max - k)
        k = max - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by the next numerator and divide by the denominator.
           All operations are integer and exact. */
        res = res * ((seq_t)(max - k + i)) / (seq_t)i;
    }
    return res;
}

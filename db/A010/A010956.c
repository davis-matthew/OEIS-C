/*
 * A010956  Binomial coefficient C(40,n).
 * https://oeis.org/A010956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binom(40,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the row. */
    if (n > 40)
        return (seq_t)0;

    /* Use symmetry C(40,n) = C(40,40-n) to minimise work. */
    idx_t k = n;
    if (k > 40 - k)
        k = 40 - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * ((seq_t)(40 - k + i)) / (seq_t)i;

    return res;
}

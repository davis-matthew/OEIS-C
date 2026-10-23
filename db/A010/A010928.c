/*
 * A010928  Binomial coefficient C(12,n).
 * https://oeis.org/A010928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(12, n) for 0 ≤ n ≤ 12, else 0
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is beyond the 12th row, the binomial coefficient is zero */
    if (n > 12)
        return (seq_t)0;

    /* Use the symmetry C(12, n) = C(12, 12-n) to minimise the number of
       multiplications. The multiplicative formula ensures that the
       intermediate results are always integers. */
    idx_t k = n;
    if (k > 12 - k)
        k = 12 - k;            /* k = min(n, 12-n) */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Multiply first, then divide. The division is exact at each step. */
        res = res * (seq_t)(12 - k + i) / (seq_t)i;
    }
    return res;
}

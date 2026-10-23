/*
 * A010939  Binomial coefficient C(23,n).
 * https://oeis.org/A010939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(23,n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n > 23, the binomial coefficient is zero. */
    if (n > 23ULL)
        return 0;

    /* Reduce the number of multiplication steps using symmetry: */
    idx_t k = n;
    if (k > 23 - k)
        k = 23 - k;

    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* r = r * (23 - k + i) / i; exact division at each step */
        r = r * (seq_t)(23 - k + i) / (seq_t)i;
    }
    return r;
}

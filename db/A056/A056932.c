/*
 * A056932  Antichains (or order ideals) in the poset 2*2*2*n or size of the
 *          distributive lattice J(2*2*2*n).
 * https://oeis.org/A056932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*C(n+8,8) - 96*C(n+7,7) + 63*C(n+6,6) - 15*C(n+5,5) +
 *            C(n+4,4) (OEIS A056932)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 34, u64 n <= 591, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t n, int k)
{
    if (k < 0 || (idx_t)k > n) return (seq_t)0;
    seq_t res = (seq_t)1;
    /* compute product iteratively: res = C(n, i) for i=0..k */
    for (int i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - (idx_t)k + (idx_t)i); /* multiply by numerator */
        res = res / (seq_t)i;                         /* divide by denominator */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t term1 = (seq_t)48 * binom_small(n + 8, 8);
    seq_t term2 = (seq_t)96 * binom_small(n + 7, 7);
    seq_t term3 = (seq_t)63 * binom_small(n + 6, 6);
    seq_t term4 = (seq_t)15 * binom_small(n + 5, 5);
    seq_t term5 = binom_small(n + 4, 4);

    seq_t result = term1 - term2 + term3 - term4 + term5;
    return result;
}

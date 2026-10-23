/*
 * A010940  Binomial coefficient C(24,n).
 * https://oeis.org/A010940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(24,n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 24, binomial coefficient is 0 */
    if (n > 24) return (seq_t)0;

    /* Use symmetry to minimize multiplications */
    idx_t k = n;
    if (k > 12) k = 24 - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(24 - k + i) / (seq_t)i;
    }
    return res;
}

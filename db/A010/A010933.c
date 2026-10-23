/*
 * A010933  Binomial coefficient C(17,n).
 * https://oeis.org/A010933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(17,n) (0 ≤ n ≤ 17, a(n)=0 for n>17)
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 17 the binomial coefficient C(17,n) is zero */
    if (n > 17)
        return (seq_t)0;

    seq_t result = 1;
    /* multiplicative computation: result = C(17,n) */
    for (idx_t k = 1; k <= n; ++k) {
        /* result = result * (17 - (n - k)) / k   */
        result = result * ((seq_t)(17 - (n - k))) / k;
    }
    return result;
}

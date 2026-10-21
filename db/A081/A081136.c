/*
 * A081136  6th binomial transform of (0,0,1,0,0,0, ...).
 * https://oeis.org/A081136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^(n-2) * (n choose 2) for n ≥ 2, a(0) = a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(0) = a(1) = 0 */

    /* compute 6^(n-2) */
    seq_t pow6 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow6 *= 6;

    /* compute n*(n-1)/2 */
    seq_t comb = (seq_t)n * (seq_t)(n - 1) / 2;

    return pow6 * comb;
}

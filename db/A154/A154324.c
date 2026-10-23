/*
 * A154324  Diagonal sums of number triangle A113582.
 * https://oeis.org/A154324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (1 + C(k+1,2) * C(n-2k+1,2))
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 331, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A154324: Diagonal sums of number triangle A113582. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t lim = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= lim; ++k)
    {
        /* C(k+1, 2) = (k+1) * k / 2 */
        seq_t ck2 = ((seq_t)(k + 1) * (seq_t)k) / 2;

        idx_t rem = n - k * 2;   /* n - 2k  */
        /* C(n-2k+1, 2) = (rem + 1) * rem / 2 */
        seq_t cn2 = ((seq_t)(rem + 1) * (seq_t)rem) / 2;

        seq_t term = 1 + ck2 * cn2;
        sum += term;
    }
    return sum;
}

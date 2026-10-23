/*
 * A138179  Wiener index of the prism graph Y_n on 2n nodes.
 * https://oeis.org/A138179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n ≥ 1, a(2k) = 4 * k^2 * (k + 1) a(2k+1) = (2k + 1) * (2k^2
 *            + 4k + 1) where k = ⌊(n-1)/2⌋ for odd n, and k = n/2 for even n
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* guards against accidental zero index */

    /* Even indices: n = 2k */
    if ((n & 1ULL) == 0)
    {
        idx_t k = n / 2;                /* use idx_t to avoid overflow */
        seq_t kk = (seq_t)k;             /* promote to seq_t once */
        return (seq_t)4 * kk * kk * (kk + 1);
    }

    /* Odd indices: n = 2k + 1 */
    {
        idx_t k = (n - 1) / 2;
        seq_t kk = (seq_t)k;
        seq_t term = (seq_t)n * ( (seq_t)2 * kk * kk + (seq_t)4 * kk + 1 );
        return term;
    }
}

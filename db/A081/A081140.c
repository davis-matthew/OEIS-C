/*
 * A081140  10th binomial transform of (0,0,1,0,0,0,...).
 * https://oeis.org/A081140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^(n-2) * binomial(n, 2), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* binomial(n,2) = n*(n-1)/2 */
    seq_t bin = (seq_t)n * ((seq_t)n - 1u) / 2u;

    /* 10^(n-2) */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow10 *= 10u;

    return bin * pow10;
}

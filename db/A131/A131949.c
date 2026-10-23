/*
 * A131949  Row sums of triangle A131948.
 * https://oeis.org/A131949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 2*n + 2*n^2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* compute 2^n */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }
    /* 2*n and 2*n^n */
    seq_t two_n  = sn * ((seq_t)2 * sn);   /* 2*n^2 */
    seq_t two_n1 = (seq_t)2 * sn;           /* 2*n */
    return pow2 + two_n1 + two_n;
}

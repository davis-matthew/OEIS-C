/*
 * A052161  Partial sums of A014825, second partial sums of A002450.
 * https://oeis.org/A052161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + C(n+2,2); a(0)=1;
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t val = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* C(i+2,2) = (i+2)*(i+1)/2 */
        seq_t tri = ((seq_t)(i + 2) * (seq_t)(i + 1)) / 2;
        val = val * 4 + tri;      /* val = 4*val + tri */
    }
    return val;
}

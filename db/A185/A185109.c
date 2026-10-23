/*
 * A185109  a(0)=2; for n > 0, a(n) = (n+2)*a(n-1) + 1.
 * https://oeis.org/A185109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*a(n-1) + 1, a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)2;             /* a(0) */
    for (idx_t i = 1; i <= n; ++i)    /* iterate up to n */
        val = ((seq_t)(i + 2) * val) + (seq_t)1;
    return val;
}

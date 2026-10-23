/*
 * A146882  a(n) = 5*(4^(n+1) - 1)/3.
 * https://oeis.org/A146882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(4^(n+1)-1)/3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)5;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = ((seq_t)4 * val) + (seq_t)5;
    }
    return val;
}

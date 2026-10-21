/*
 * A218728  a(n) = (25^n - 1)/24.
 * https://oeis.org/A218728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    for (idx_t i = (idx_t)0; i < n; ++i)
    {
        res = res * ((seq_t)25) + (seq_t)1;
    }
    return res;
}

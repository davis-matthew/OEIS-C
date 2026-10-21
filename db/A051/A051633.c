/*
 * A051633  a(n) = 5*2^n - 2.
 * https://oeis.org/A051633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*2^n - 2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i)
        pow2 *= (seq_t)2;
    return ((seq_t)5 * pow2) - (seq_t)2;
}

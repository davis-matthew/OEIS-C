/*
 * A161212  a(n) = A161210(n)*2.
 * https://oeis.org/A161212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = { n*(n+1) for n<3 ; 14*n-28 for n>=3 }
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)n * ((seq_t)n + 1);
    else
        return ((seq_t)n * 14) - 28;
}

/*
 * A093544  Numerator of (4*n-3)/A000265(n). Numerator of pairwise quotients
 *          of A004130.
 * https://oeis.org/A093544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = If[Mod[n,3]==0, (4 n-3)/3, 4 n-3]
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 4 - 3;
    return (n % 3 == 0) ? (val / 3) : val;
}

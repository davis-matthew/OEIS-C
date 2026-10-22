/*
 * A001969  Evil numbers: nonnegative integers with an even number of 1's in
 *          their binary expansion.
 * https://oeis.org/A001969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, core, nonn, nice, base
 * Formula:   a(n) = 2n - 1 if popcount(n-1) is odd, else 2n - 2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int pc = __builtin_popcountll(n - 1);
    seq_t nn = (seq_t)n;
    return nn * 2 - ((pc & 1) ? 1 : 2);
}

/*
 * A188861  Number of n X 4 binary arrays without the pattern 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=16; a(2)=41; a(3)=68; a(n)=27*n-13 for n>=4
 * Range:     u8 n <= 9, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)41;
    if (n == 3) return (seq_t)68;
    return ((seq_t)27 * (seq_t)n) - (seq_t)13;
}

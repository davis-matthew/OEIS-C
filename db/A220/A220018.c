/*
 * A220018  Number of cyclotomic cosets of 3 mod 10^n.
 * https://oeis.org/A220018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=4, a(2)=11; a(n)=8*n^2-14*n-1 for n>2
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n == 1ULL) return 4;
    if (n == 2ULL) return 11;
    return 8 * nn * nn - 14 * nn - 1;
}

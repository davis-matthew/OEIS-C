/*
 * A019426  Continued fraction for tan(1/3).
 * https://oeis.org/A019426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(n) = 0 for n=0, a(1)=2, a(n)=1 if n even, a(n)=3*n-2 if n odd
 *            and n>1
 * Range:     u8 n <= 86, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 9989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)2;
    if ((n & (idx_t)1) == (idx_t)0) return (seq_t)1;
    return ((seq_t)3 * (seq_t)n) - (seq_t)2;
}

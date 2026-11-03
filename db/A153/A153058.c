/*
 * A153058  a(0)=4; a(n)=n^2+a(n-1) for n>0.
 * https://oeis.org/A153058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 + n*(n+1)*(2*n+1)/6
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1439, u64 n <= 64950,
 *            u128 n <= 64950, big any n
 * Validated: standard, pass, 59 terms through n = 9066 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t term = ns * (ns + (seq_t)1) * ((seq_t)2 * ns + (seq_t)1);
    term /= (seq_t)6;
    return term + (seq_t)4;
}

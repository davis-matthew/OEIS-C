/*
 * A117665  n times the n-th n-gonal number.
 * https://oeis.org/A117665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2-3*n+4)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = sn * (sn + 1);
    term *= (sn * sn - 3 * sn + 4);
    term /= 2;
    return term;
}

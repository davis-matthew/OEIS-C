/*
 * A079028  a(0) = 1, a(n) = (n + 4)*4^(n-1) for n >= 1.
 * https://oeis.org/A079028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4) * 4^(n-1) for n>=1; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)   /* compute 4^(n-1) */
        power *= (seq_t)4;
    return ((seq_t)(n + 4)) * power;
}

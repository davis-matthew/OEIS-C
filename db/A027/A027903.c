/*
 * A027903  a(n) = n*(n + 1)*(3*n + 1).
 * https://oeis.org/A027903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n+1)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    return sn * (sn + 1) * (3 * sn + 1);
}

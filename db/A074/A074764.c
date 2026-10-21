/*
 * A074764  Numbers of smaller squares into which a square may be dissected.
 * https://oeis.org/A074764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+3 for all n > 2, with the special cases a(1)=1 and
 *            a(2)=4
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    return (seq_t)n + (seq_t)3;
}

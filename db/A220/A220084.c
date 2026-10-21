/*
 * A220084  a(n) = (n + 1)*(20*n^2 + 19*n + 6)/6.
 * https://oeis.org/A220084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(20*n^2 + 19*n + 6)/6
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 610, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t term = (seq_t)20 * n2 + (seq_t)19 * sn + (seq_t)6;
    seq_t res  = ((sn + 1) * term) / (seq_t)6;
    return res;
}

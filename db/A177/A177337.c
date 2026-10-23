/*
 * A177337  Partial sums of round(n^2/36).
 * https://oeis.org/A177337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+5)*(2*n^2 - 7*n + 17)/216)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 910, u64 n <= 910,
 *            u128 n <= 910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t inner = (2 * sn * sn) - (7 * sn) + (seq_t)17;
    seq_t num   = (sn + 5) * inner;
    return num / (seq_t)216;
}

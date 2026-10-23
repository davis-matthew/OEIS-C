/*
 * A045945  Hexagonal matchstick numbers: a(n) = 3*n*(3*n+1).
 * https://oeis.org/A045945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(3*n+1)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before any arithmetic to avoid overflow of idx_t */
    seq_t sn      = (seq_t)n;
    seq_t three_n = sn * (seq_t)3;
    return three_n * (three_n + (seq_t)1);
}

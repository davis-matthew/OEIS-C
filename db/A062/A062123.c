/*
 * A062123  a(n) = (9n^2 + 9n + 4)/2.
 * https://oeis.org/A062123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 9*n + 4)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is uint64_t (idx_t). Convert to seq_t before arithmetic. */
    seq_t sn = (seq_t)n;
    /* Compute 9*n^2 + 9*n + 4, guaranteeing intermediate in seq_t. */
    return (9 * sn * sn + 9 * sn + 4) / 2;
}

/*
 * A154379  a(n) = 250*n + 10.
 * https://oeis.org/A154379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 250*n + 10
 * Range:     u8 none, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    return (seq_t)250 * (seq_t)n + (seq_t)10;
}

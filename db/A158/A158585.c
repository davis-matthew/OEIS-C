/*
 * A158585  a(n) = 289*n^2 + 17.
 * https://oeis.org/A158585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 289*n^2 + 17
 * Range:     u8 n <= 0, u16 n <= 15, u32 n <= 3855, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return 289 * x * x + 17;
}

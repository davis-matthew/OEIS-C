/*
 * A017007  a(n) = (7*n + 2)^3.
 * https://oeis.org/A017007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^3
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 231, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (7*n + 2) in seq_t, then cube */
    seq_t x = (seq_t)n * (seq_t)7 + (seq_t)2;
    return x * x * x;
}

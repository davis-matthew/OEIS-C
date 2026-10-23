/*
 * A133368  Period 5: 1, 1, 3, 7, 3.
 * https://oeis.org/A133368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - n + 1 (mod 10)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 10 first; the expression depends only on this. */
    idx_t r = n % 10;
    seq_t rr = (seq_t)r;
    seq_t v = rr * rr - rr + 1;
    return v % 10;
}

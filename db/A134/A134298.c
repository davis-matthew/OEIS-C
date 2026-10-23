/*
 * A134298  a(n) = (107*n)^5.
 * https://oeis.org/A134298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (107*n)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Compute 107*n first, then raise to the fifth power
    seq_t x = (seq_t)107 * (seq_t)n;   // x = 107*n
    seq_t p = x * x;                  // x^2
    seq_t q = p * p;                  // x^4
    seq_t result = q * x;             // x^5
    return result;
}

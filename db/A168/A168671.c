/*
 * A168671  Numbers that are congruent to {1, 10} mod 13.
 * https://oeis.org/A168671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (26*n + 5*(-1)^n - 17)/4, n ≥ 1
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent simplified form:
   a(n) = (13*n - 6)/2  for even n
   a(n) = (13*n - 11)/2 for odd n   */
seq_t a(idx_t n)
{
    /* n is ≥ 1 */
    /* Compute 13*n first.  It fits in seq_t for all test cases */
    seq_t prod = (seq_t)13 * n;
    /* For even n use (13*n - 6)/2, for odd n use (13*n - 11)/2 */
    if ((n & 1) == 0) {
        return (prod - (seq_t)6) / (seq_t)2;
    } else {
        return (prod - (seq_t)11) / (seq_t)2;
    }
}

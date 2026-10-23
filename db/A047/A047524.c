/*
 * A047524  Numbers that are congruent to {2, 7} mod 8.
 * https://oeis.org/A047524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (n mod 2) - 1
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t to avoid overflow in intermediate ops */
    return nn * 4 - (nn & 1) - 1; /* (nn & 1) is equivalent to (nn % 2) */
}

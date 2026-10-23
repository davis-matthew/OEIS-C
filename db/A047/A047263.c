/*
 * A047263  Numbers that are congruent to {0, 1, 2, 4, 5} mod 6.
 * https://oeis.org/A047263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-4)/5) Equivalent to: a(n) = n + (n+1)/5 - 1,
 *            using unsigned arithmetic to avoid negative values for n < 4.
 *            a(1) = 0, a(2) = 1, a(3) = 2, etc
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t block = (nn + (seq_t)1) / (seq_t)5;   /* floor((n+1)/5) */
    return nn + block - (seq_t)1;               /* a(n) = n + block - 1 */
}

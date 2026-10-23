/*
 * A047268  Numbers that are congruent to {1, 2, 5} mod 6.
 * https://oeis.org/A047268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 - floor((n mod 3)/2)
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    idx_t m = n % 3ULL;                     /* n mod 3 */
    seq_t sub = (m == 2) ? (seq_t)1 : (seq_t)0;  /* floor((n mod 3)/2) */
    return sn * 2 - 1 - sub;
}

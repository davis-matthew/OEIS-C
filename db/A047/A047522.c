/*
 * A047522  Numbers that are congruent to {1, 7} mod 8.
 * https://oeis.org/A047522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/2)+1 if n is odd, a(n) = 8*(n/2)-1 if n is even
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n & 1) {
        seq_t k = nn / 2;          /* floor((n-1)/2) */
        return k * (seq_t)8 + 1;
    } else {
        seq_t k = nn / 2;          /* n/2 */
        return k * (seq_t)8 - 1;
    }
}

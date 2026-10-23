/*
 * A047476  Numbers that are congruent to {0, 1, 2, 3} mod 8.
 * https://oeis.org/A047476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4)+((n-1)%4) // A047476, offset 1
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t adj = (seq_t)(n - 1);   /* adjust for offset 1 */
    seq_t k   = adj / 4;          /* floor((n-1)/4)   */
    seq_t r   = adj % 4;          /* (n-1) mod 4      */
    return k * 8 + r;
}

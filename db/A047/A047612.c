/*
 * A047612  Numbers that are congruent to {0, 2, 4, 5} mod 8.
 * https://oeis.org/A047612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [0,2,4,5][(n-1)%4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 4;          /* block index */
    idx_t r = (n - 1) % 4;          /* position within block */
    static const seq_t rem[4] = { 0, 2, 4, 5 };
    return (seq_t)(8 * k) + rem[r];
}

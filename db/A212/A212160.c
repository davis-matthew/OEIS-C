/*
 * A212160  Numbers that are congruent to {2, 10} mod 13.
 * https://oeis.org/A212160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 13*n + 2, a(2*n+1) = 13*n + 10
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                /* n = 2*k  */
        return (seq_t)13 * (seq_t)k + 2;
    } else {
        idx_t k = (n - 1) >> 1;          /* n = 2*k+1 */
        return (seq_t)13 * (seq_t)k + 10;
    }
}

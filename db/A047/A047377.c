/*
 * A047377  Numbers that are congruent to {0, 1, 4, 5} mod 7.
 * https://oeis.org/A047377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + [0,1,4,5][(n-1)%4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition, so n-1 is valid */
    const idx_t n1 = n - 1;
    const idx_t group = n1 / 4;          /* number of complete blocks of 7 */
    const idx_t r = n1 % 4;              /* position inside the current block */
    static const seq_t vals[4] = { 0, 1, 4, 5 };
    return (seq_t)group * (seq_t)7 + vals[r];
}

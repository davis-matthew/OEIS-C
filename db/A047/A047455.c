/*
 * A047455  Numbers that are congruent to {2, 3, 4} mod 8.
 * https://oeis.org/A047455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n - 1) / 3) + ((n - 1) % 3 + 2)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, return 0 as sentinel */
    idx_t q = (n - 1) / 3;
    idx_t r = (n - 1) % 3;
    return (seq_t)8 * (seq_t)q + (seq_t)(r + 2);
}

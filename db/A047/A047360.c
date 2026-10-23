/*
 * A047360  Numbers that are congruent to {1, 2, 3} mod 7.
 * https://oeis.org/A047360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + ((n-1) mod 3) + 1; offset 1
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* values for n<1 are undefined */
    seq_t k = (seq_t)((n - 1) / 3);
    seq_t r = (seq_t)((n - 1) % 3);
    return 7 * k + r + 1;
}

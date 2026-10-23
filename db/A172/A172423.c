/*
 * A172423  Period 10: repeat [0, 9, 2, 7, 4, 5, 6, 3, 8, 1].
 * https://oeis.org/A172423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pattern[n mod 10]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A172423: Period 10 repeating [0, 9, 2, 7, 4, 5, 6, 3, 8, 1] */

seq_t a(idx_t n)
{
    static const unsigned char pattern[10] = {0, 9, 2, 7, 4, 5, 6, 3, 8, 1};
    return (seq_t)pattern[n % 10];
}

/*
 * A060858  "0303456", a popular song by Rafaela Carra.
 * https://oeis.org/A060858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = pattern[n mod 7] where pattern = {0,3,0,3,4,5,6}
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[7] = {0, 3, 0, 3, 4, 5, 6};
    return pattern[n % 7];
}

/*
 * A220154  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 2 X n array.
 * https://oeis.org/A220154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*n*n - 23*n + 16)/2 for n>2; * a(1) = 6, a(2) = 11
 * Range:     u8 n <= 5, u16 n <= 80, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)11;

    seq_t sn = (seq_t)n;
    seq_t numerator = (seq_t)11 * sn * sn - (seq_t)23 * sn + (seq_t)16;
    return numerator / (seq_t)2;
}

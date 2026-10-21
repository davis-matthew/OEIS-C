/*
 * A219498  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 4 array.
 * https://oeis.org/A219498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 6*n^2 + 23*n - 42)/6 for n > 2 a(1) = 4 a(2) = 7
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 + (seq_t)6 * n2 + (seq_t)23 * nn - (seq_t)42;
    return num / (seq_t)6;
}

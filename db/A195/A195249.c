/*
 * A195249  Number of lower triangles of a 3 X 3 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by two or less.
 * https://oeis.org/A195249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1047*n - 1459 for n >= 3 Special cases: a(1) = 64 a(2) =
 *            729 a(3) = 1682 a(4) = 2729 Recurrence a(n) = 2*a(n-1) - a(n-2)
 *            holds for n > 4 and is equivalent to the linear formula
 * Range:     u8 n <= 1, u16 n <= 63, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)64;
        case 2: return (seq_t)729;
        case 3: return (seq_t)1682;
        case 4: return (seq_t)2729;
        default: return (seq_t)1047 * (seq_t)n - (seq_t)1459;
    }
}

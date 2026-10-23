/*
 * A190037  Number of nondecreasing arrangements of n+2 numbers in 0..5 with
 *          the last equal to 5 and each after the second equal to the sum of
 *          one or two of the preceding three.
 * https://oeis.org/A190037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8, a(2) = 12, a(3) = 16, a(4) = 23, a(n) = 12*n - 27 for
 *            n>4
 * Range:     u8 n <= 23, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)23;

    /* For n > 4 use the linear formula */
    return ((seq_t)n * (seq_t)12) - (seq_t)27;
}

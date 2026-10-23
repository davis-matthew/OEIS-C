/*
 * A190038  Number of nondecreasing arrangements of n+2 numbers in 0..6 with
 *          the last equal to 6 and each after the second equal to the sum of
 *          one or two of the preceding three.
 * https://oeis.org/A190038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 - 8*n + 11 for n>5. Initial values: a(1)=10,
 *            a(2)=18, a(3)=30, a(4)=47, a(5)=72
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = {10, 18, 30, 47, 72};

    if (n == 0) return 0;          /* offset is 1, but return 0 for safety */
    if (n <= 5) return init[n - 1]; /* first five terms */

    seq_t nn = (seq_t)n;
    return 4 * nn * nn - 8 * nn + 11;
}

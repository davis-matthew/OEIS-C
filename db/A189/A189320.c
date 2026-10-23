/*
 * A189320  Number of nondecreasing arrangements of n+2 numbers in 0..3 with
 *          the last equal to 3 and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 8 for n > 3 Initial values: a(1)=5, a(2)=7, a(3)=11
 * Range:     u8 n <= 43, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial three terms explicitly */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)11;

    /* For n >= 4, use the empirical linear formula */
    return ((seq_t)n * 6u) - 8u;
}

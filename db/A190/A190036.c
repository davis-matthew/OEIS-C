/*
 * A190036  Number of nondecreasing arrangements of n+2 numbers in 0..4 with
 *          the last equal to 4 and each after the second equal to the sum of
 *          one or two of the preceding three.
 * https://oeis.org/A190036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+3) - 1 for n > 3; a(1) = 7, a(2) = 12, a(3) = 18
 * Range:     u8 n <= 14, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, return 0 as a placeholder */
    if (n == 1)
        return (seq_t)7;
    if (n == 2)
        return (seq_t)12;
    if (n == 3)
        return (seq_t)18;
    /* For n >= 4 use the closed formula */
    return (seq_t)n * ((seq_t)n + 3) - 1;
}

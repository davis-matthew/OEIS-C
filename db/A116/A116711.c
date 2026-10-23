/*
 * A116711  Number of permutations of length n which avoid the patterns 123,
 *          3214, 4312.
 * https://oeis.org/A116711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2*n - 12 (for n >= 4) Initial values: a(1) = 1,
 *            a(2) = 2, a(3) = 5
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset < 1 (undefined in OEIS, return 0 for safety) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* For n >= 4 use the quadratic closed form */
    return (seq_t)n * (seq_t)n
         + (seq_t)2 * (seq_t)n
         - (seq_t)12;
}

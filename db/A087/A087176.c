/*
 * A087176  a(n) - 1 is the maximal number of integers for which the absolute
 *          value of a reducible polynomial with integral coefficients of
 *          degree n is a prime.
 * https://oeis.org/A087176
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = n + 3 for n >= 7 with the known special values a(2) = 5,
 *            a(3) = 6, a(4) = 9, a(5) = 9, a(6) = 9 (offset starts at n = 2)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)6;
    if (n == 4 || n == 5 || n == 6) return (seq_t)9;
    if (n >= 7) return (seq_t)n + (seq_t)3;
    /* n < 2: not defined in the OEIS offset; return 0 */
    return (seq_t)0;
}

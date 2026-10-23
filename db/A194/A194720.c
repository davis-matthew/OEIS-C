/*
 * A194720  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting eight doublets into the
 *          initially empty word.
 * https://oeis.org/A194720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0 * a(n) = 1 + (14 + (90 + (350 + (910 + (1638 + (2002
 *            + 1430*(n-1))*(n-1))*(n-1))*(n-1))*(n-1))*(n-1))*(n-1) * for n >
 *            0 * This is a polynomial of degree 8 in n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 201, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t m = (seq_t)(n - 1);   /* m = n-1 */
    seq_t t = 1430;             /* start with the constant of the innermost term */
    t = 2002 + t * m;
    t = 1638 + t * m;
    t = 910 + t * m;
    t = 350 + t * m;
    t = 90  + t * m;
    t = 14  + t * m;
    return 1 + t * m;
}

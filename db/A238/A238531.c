/*
 * A238531  Expansion of (1 - x + x^2)^2 / (1 - x)^3 in powers of x.
 * https://oeis.org/A238531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 4)/2 for n ≥ 2, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 15, u16 n <= 325, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two exceptional initial values */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute the quadratic expression for n ≥ 2 */
    seq_t nn = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t res = (nn - (seq_t)n + (seq_t)4) / (seq_t)2;
    return res;
}

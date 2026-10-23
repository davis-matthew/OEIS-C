/*
 * A155138  a(n) = nonnegative value y such that (A155136(n), y) is a solution
 *          to the Diophantine equation x^3+28*x^2 = y^2.
 * https://oeis.org/A155138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |(n-1)^3 - 28*(n-1)| (OEIS A155138)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t and shift to 0‑based value m = n-1 */
    seq_t m = (seq_t)n - 1;

    /* Compute m^3 and 28*m */
    seq_t m3 = m * m * m;
    seq_t t28m = (seq_t)28 * m;

    /* Return the absolute difference */
    if (m3 >= t28m)
        return m3 - t28m;
    else
        return t28m - m3;
}

/*
 * A077966  Expansion of 1/(1+2*x^2).
 * https://oeis.org/A077966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1+(-1)^n)*(-2)^(n/2)/2, equivalent to a(2k) = (-2)^k,
 *            a(2k+1) = 0
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero for odd indices */
    if (n & 1U)
        return 0;

    idx_t k = n >> 1;          /* compute k = n/2 */
    seq_t res = 1;             /* result accumulator */

    /* Compute (-2)^k by repeated multiplication */
    for (idx_t i = 0; i < k; ++i)
        res = res * (seq_t)(-2);

    return res;
}

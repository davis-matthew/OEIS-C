/*
 * A138897  Ratio of (2n-1)! to number of zeros in upper part of Sylvester
 *          matrix of polynomial of degree n with all nonzero coefficients.
 * https://oeis.org/A138897
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n - 1)! / (n * (n - 1)) (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 2; return 0 for smaller indices. */
    if (n < 2) return 0;

    idx_t m = 2 * n - 1;          /* upper limit of the factorial product   */
    seq_t result = 1;             /* running product of remaining factors  */

    /* Multiply all integers from 1 to m, skipping the two
       factors n and (n - 1) which appear in the denominator.   */
    for (idx_t k = 1; k <= m; ++k) {
        if (k == n || k == n - 1) continue;
        result *= (seq_t)k;
    }

    return result;
}

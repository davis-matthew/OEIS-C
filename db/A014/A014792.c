/*
 * A014792  Squares of even heptagonal numbers.
 * https://oeis.org/A014792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (200*n^4 - 120*n^3 + 18*n^2)/2 if n is even a(n) =
 *            (200*n^4 + 280*n^3 + 138*n^2 + 28*n + 2)/2 if n is odd (Computed
 *            directly from the piecewise formulas in the OEIS entry.)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n once to the output type to keep all intermediate
       arithmetic in type seq_t */
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;          /* n^2   */
    seq_t n3 = n2 * s;         /* n^3   */
    seq_t n4 = n2 * n2;        /* n^4   */

    if ((n & 1) == 0) { /* even n */
        seq_t num = (200 * n4) - (120 * n3) + (18 * n2);
        return num / 2;
    } else {            /* odd n */
        seq_t num = (200 * n4) + (280 * n3) + (138 * n2) + (28 * s) + 2;
        return num / 2;
    }
}

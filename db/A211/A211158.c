/*
 * A211158  Number of 2 X 2 matrices having all terms in {-n,...,0,..,n} and
 *          positive odd determinant.
 * https://oeis.org/A211158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n + 1) * (3*n + 1) for even n a(n) = n * (n + 1)^2
 *            * (3*n + 2) for odd n (produced by the algebraic formula for
 *            A211158)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 194, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t nn  = (seq_t)n;   /* n */
    seq_t np1 = nn + 1;     /* n+1 */

    if ((n & 1ULL) == 0) {          /* even n */
        /* n^2*(n+1)*(3*n+1) */
        return (nn * nn) * np1 * ((seq_t)3 * nn + (seq_t)1);
    } else {                        /* odd n */
        /* n*(n+1)^2*(3*n+2) */
        return nn * (np1 * np1) * ((seq_t)3 * nn + (seq_t)2);
    }
}

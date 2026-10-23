/*
 * A121781  Series expansion for mean-squared radius of gyration of Ferrers
 *          diagrams on square lattice.
 * https://oeis.org/A121781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1; for n >= 2, a(n) = 2^(n-7)*(21*n^4 + 50*n^3
 *            + 3*n^2 + 70*n - 120)/3 (integer division by 3 is exact)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 19, u64 n <= 46, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    /* Compute the polynomial 21*n^4 + 50*n^3 + 3*n^2 + 70*n - 120 */
    seq_t expr = 21 * n4 + 50 * n3 + 3 * n2 + 70 * nn - 120;

    /* Division by 3 is exact according to the OEIS formula */
    seq_t val = expr / (seq_t)3;

    if (n >= 7) {
        val <<= (n - 7);            /* multiply by 2^(n-7) */
    } else {
        val >>= (7 - n);            /* divide by 2^(7-n) */
    }
    return val;
}

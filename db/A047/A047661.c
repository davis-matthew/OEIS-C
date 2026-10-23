/*
 * A047661  Row 5 of square array defined in A047662.
 * https://oeis.org/A047661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^4 + 5*n^3 + 20*n^2 + 25*n + 23) / 15
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the function returns 0, though the sequence is defined for n ≥ 1 */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;           /* n as seq_t */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n2 * n2;            /* n^4 */

    /* Polynomial p = 2*n^4 + 5*n^3 + 20*n^2 + 25*n + 23 */
    seq_t p = 2 * n4 + 5 * n3 + 20 * n2 + 25 * nn + 23;

    /* Compute (nn * p) / 15 in a way that avoids overflow:
       (nn / 15) * p  +  (nn % 15) * p / 15  */
    seq_t q = nn / 15;
    seq_t r = nn % 15;
    seq_t res = q * p + (r * p) / 15;

    return res;
}

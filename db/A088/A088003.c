/*
 * A088003  Take the list t(n,0) = {1,...,n}; denote by t(n,j) this list after
 *          rotating to left (or right) by j positions. Calculate inner
 *          product of t(n,0) and t(n,j) and denote the value by s(n,j).
 *          Compute this inner product for all j = 1..n and choose the
 *          smallest. This is a(n).
 * https://oeis.org/A088003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n*(5*n+12)-3*(-1)^n+11)/48
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* value of n in the coefficient type */
    seq_t N = (seq_t)n;

    /* compute the inner expression using unsigned arithmetic.
       The term 2*N*(5*N+12) is always >= 42 for n >= 1, so
       the subtraction of 3 (for even n) never underflows. */
    seq_t expr = 2 * N * (5 * N + 12);

    /* adjust by -3 for even n, +3 for odd n */
    if ((n & 1) == 0)
        expr -= 3;          /* n even  -> -3 */
    else
        expr += 3;          /* n odd   -> +3 */

    expr += 11;              /* final constant 11 */

    /* final division is exact for all n >= 1 */
    return N * expr / 48;
}

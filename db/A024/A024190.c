/*
 * A024190  [ (2nd elementary symmetric function of 3,4,...,n+3)/(3+4+...+n+3)
 *          ].
 * https://oeis.org/A024190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((( (n+1)(n+6)/2 )^2 - ((n+3)(n+4)(2n+7)/6 - 5 )) /
 *            (2 * (n+1)(n+6)/2))
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 670, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t N = (seq_t)n;

    /* Sum of the arithmetic progression 3,4,…,n+3 */
    seq_t S = (N + 1) * (N + 6) / 2;                /* S = (n+1)(n+6)/2 */

    /* Sum of squares of that progression */
    seq_t P = ((N + 3) * (N + 4) * (2 * N + 7) / 6) - 5; /* P = (n+3)(n+4)(2n+7)/6 - 5 */

    /* Numerator: S^2 - P */
    seq_t num = S * S - P;

    /* Denominator: 2*S */
    seq_t denom = 2 * S;

    /* Integer division truncates toward zero, i.e. floor for positives */
    return num / denom;
}

/*
 * A227265  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having a sum of three or less, with
 *          rows and columns of the latter in lexicographically nondecreasing
 *          order.
 * https://oeis.org/A227265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2 n^3 – n^2 + 94 n – 24) / 24
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = (1/24)*n^4 + (1/12)*n^3 – (1/24)*n^2 + (47/12)*n – 1 */

seq_t a(idx_t n)
{
    /* cast the index to bigint first */
    seq_t N = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = N * N;      /* n^2 */
    seq_t n3 = n2 * N;     /* n^3 */
    seq_t n4 = n3 * N;     /* n^4 */

    /* build the numerator: n^4 + 2*n^3 + 94*n – n^2 – 24 */
    seq_t num = n4;
    num += 2 * n3;         /* + 2 n^3 */
    num += 94 * N;         /* + 94 n */
    num -= n2;             /* – n^2 */
    num -= 24;             /* – 24 */

    /* divide by 24 to get the term */
    return num / 24;
}

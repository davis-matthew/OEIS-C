/*
 * A052459  a(n) = n*(2*n^2 + 1)*(n^2 + 1)/6.
 * https://oeis.org/A052459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 1)*(n^2 + 1)/6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 105, u64 n <= 8883,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2    = n_seq * n_seq;       /* n^2                           */
    seq_t y     = 2 * n2 + 1;           /* 2*n^2 + 1                     */
    seq_t z     = n2 + 1;               /* n^2 + 1                       */

    /* cancel the factor 2 */
    if (n_seq % 2 == 0)
        n_seq /= 2;
    else if (y % 2 == 0)
        y /= 2;
    else
        z /= 2;

    /* cancel the factor 3 */
    if (n_seq % 3 == 0)
        n_seq /= 3;
    else if (y % 3 == 0)
        y /= 3;
    else
        z /= 3;

    return n_seq * y * z;
}

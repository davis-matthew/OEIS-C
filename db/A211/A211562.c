/*
 * A211562  Number of nonnegative integer arrays of length n+2 with new values
 *          0 upwards introduced in order, and containing the value n-1.
 * https://oeis.org/A211562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^4 + 10*n^3 + 21*n^2 + 38*n + 48) / 24
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 198, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (1/8)*n^4 + (5/12)*n^3 + (7/8)*n^2 + (19/12)*n + 2 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;        /* n^2 */
    seq_t x3 = x2 * x;       /* n^3 */
    seq_t x4 = x3 * x;       /* n^4 */

    /* Numerator: 3*n^4 + 10*n^3 + 21*n^2 + 38*n + 48 */
    seq_t num = (seq_t)3 * x4;
    num += (seq_t)10 * x3;
    num += (seq_t)21 * x2;
    num += (seq_t)38 * x;
    num += (seq_t)48;

    /* Exact division by 24 */
    return num / (seq_t)24;
}

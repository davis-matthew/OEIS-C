/*
 * A174642  Number of ways to place 4 nonattacking amazons (superqueens) on a
 *          4 X n board.
 * https://oeis.org/A174642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-7)*(n^3-21n^2+158n-420) for n >= 7, otherwise 0
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 262, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7U)
        return 0;
    seq_t t  = (seq_t)n;
    seq_t t2 = t * t;          /* n^2 */
    seq_t t3 = t2 * t;         /* n^3 */
    seq_t t4 = t3 * t;         /* n^4 */

    seq_t result = t4;                     /* + n^4 */
    result -= (seq_t)28U * t3;             /* - 28*n^3 */
    result += (seq_t)305U * t2;            /* + 305*n^2 */
    result -= (seq_t)1526U * t;            /* - 1526*n */
    result += 2940U;                      /* + 2940 */

    return result;
}

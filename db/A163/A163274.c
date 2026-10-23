/*
 * A163274  a(n) = n^4*(n+1)^2/2.
 * https://oeis.org/A163274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4*(n+1)^2/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;          /* convert index to seq_t */
    seq_t nn1 = nn + 1;            /* n + 1 */

    /* Compute n^4 */
    seq_t term = nn * nn;          /* n^2 */
    term = term * term;            /* n^4 */

    /* Compute (n+1)^2 */
    seq_t term2 = nn1 * nn1;       /* (n+1)^2 */

    /* Multiply and divide by 2 */
    term = term * term2;           /* n^4 * (n+1)^2 */
    term = term / (seq_t)2;        /* division by 2 */

    return term;
}

/*
 * A134160  a(n) = 163 + 1053*n + 2520*n^2 + 2646*n^3 + 1029*n^4.
 * https://oeis.org/A134160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 163 + 1053*n + 2520*n^2 + 2646*n^3 + 1029*n^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* convert index to seq_t */
    seq_t res = 1029;            /* coefficient of n^4   */
    res = res * x + 2646;        /* add 2646*n^3         */
    res = res * x + 2520;        /* add 2520*n^2         */
    res = res * x + 1053;        /* add 1053*n           */
    res = res * x + 163;         /* add constant 163     */
    return res;
}

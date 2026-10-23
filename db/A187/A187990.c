/*
 * A187990  Number of nondecreasing arrangements of 6 numbers x(i) in
 *          -(n+4)..(n+4) with the sum of sign(x(i))*2^|x(i)| zero.
 * https://oeis.org/A187990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 39*n^2 + 260*n + 402)/6
 * Range:     u8 none, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to sequence type */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    seq_t part1 = n3;                            /* n^3 */
    seq_t part2 = (seq_t)39 * n2;                /* 39*n^2 */
    seq_t part3 = (seq_t)260 * nn;               /* 260*n */

    seq_t sum = part1 + part2 + part3 + (seq_t)402;
    return sum / (seq_t)6;
}

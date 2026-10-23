/*
 * A157614  a(n) = 29282*n^2 + 484*n + 1.
 * https://oeis.org/A157614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29282*n^2 + 484*n + 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 382, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;          /* cast index to seq_t */
    seq_t N2  = N * N;             /* n^2 */
    seq_t res = 29282 * N2;        /* 29282 * n^2 */
    res      += 484 * N;           /* + 484 * n */
    res      += 1;                 /* + 1 */
    return res;
}

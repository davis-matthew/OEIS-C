/*
 * A081489  a(n) = n*(2*n^2 -3*n +7)/6 = C(n, 1) + C(n, 2) + 2*C(n, 3).
 * https://oeis.org/A081489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 - 3*n + 7)/6
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* cast index to sequence type */
    seq_t tmp = (seq_t)2 * nn * nn; /* 2*n^2 */
    tmp = tmp - (seq_t)3 * nn + (seq_t)7;  /* 2*n^2 - 3*n + 7 */
    tmp = nn * tmp;               /* n * (2*n^2 - 3*n + 7) */
    return tmp / (seq_t)6;        /* divide by 6 (exact) */
}

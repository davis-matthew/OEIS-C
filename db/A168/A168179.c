/*
 * A168179  a(n) = n^3*(n^4 + 1)/2.
 * https://oeis.org/A168179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * (n^4 + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 480, u128 n <= 480,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the arithmetic type used for values */
    seq_t n_seq = (seq_t)n;

    /* Compute powers of n using only integer arithmetic */
    seq_t n_sq = n_seq * n_seq;        /* n^2   */
    seq_t n_cu = n_sq * n_seq;         /* n^3   */
    seq_t n_4  = n_sq * n_sq;          /* n^4   */

    /* Compute the required expression: n^3 * (n^4 + 1) / 2 */
    seq_t sum  = n_4 + (seq_t)1;       /* n^4 + 1 */
    seq_t prod = n_cu * sum;           /* n^3 * (n^4 + 1) */
    seq_t res  = prod / (seq_t)2;      /* divide by 2 (exactly) */

    return res;
}

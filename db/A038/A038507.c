/*
 * A038507  a(n) = n! + 1.
 * https://oeis.org/A038507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n! + 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                     /* start with 1 (0! = 1) */
    for (idx_t i = 2; i <= n; ++i) {   /* multiply 2*3*...*n */
        res *= (seq_t)i;                /* cast index to seq_t before multiplication */
    }
    return res + 1;                    /* a(n) = n! + 1 */
}

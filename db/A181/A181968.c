/*
 * A181968  a(n) = 54n^3 - 1.
 * https://oeis.org/A181968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 54*n^3 - 1
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 430, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s  = (seq_t)n;         /* Convert n to seq_t */
    seq_t n2   = n_s * n_s;        /* n^2 */
    seq_t n3   = n2 * n_s;         /* n^3 */
    seq_t res  = 54 * n3 - 1;       /* 54*n^3 - 1 */
    return res;
}

/*
 * A207399  Number of n X 4 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 6*n^3 + 7*n^2 - 6*n + 1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = s * s;            /* n^2 */
    seq_t n3 = n2 * s;           /* n^3 */
    seq_t n4 = n3 * s;           /* n^4 */
    seq_t res = n4 + (seq_t)6 * n3 + (seq_t)7 * n2 + (seq_t)1;
    res -= (seq_t)6 * s;         /* subtract 6*n */
    return res;
}

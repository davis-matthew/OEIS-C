/*
 * A180355  a(n) = n^5 + 5n.
 * https://oeis.org/A180355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 5*n
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A180355 for all n >= 0 */
seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;      /* Convert index to sequence type */
    seq_t res = n_s;           /* Start with n^1 */
    res *= n_s;                /* n^2 */
    res *= n_s;                /* n^3 */
    res *= n_s;                /* n^4 */
    res *= n_s;                /* n^5 */
    res += (seq_t)5 * n_s;     /* +5*n */
    return res;
}

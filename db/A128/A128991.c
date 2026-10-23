/*
 * A128991  a(n) = (n^3 - n^2)*8^n.
 * https://oeis.org/A128991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n^3 - n^2 */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;          /* n^2 */
    seq_t n_cb  = n_sq * n_seq;           /* n^3 */
    seq_t part  = n_cb - n_sq;            /* n^3 - n^2 */

    /* 8^n */
    seq_t pow8 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow8 *= (seq_t)8;

    return part * pow8;
}

/*
 * A127873  a(n) = (n^3)/2 + (3*n^2)/2 + 3*n + 3.
 * https://oeis.org/A127873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3)/2 + (3*n^2)/2 + 3*n + 3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1895, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq    = n_seq * n_seq;          /* n^2 */
    seq_t numer   = n_sq * (n_seq + 3);     /* n^2 * (n+3) = n^3 + 3*n^2 */
    seq_t part    = numer / 2;              /* integer division, exact */
    seq_t result  = part + 3 * n_seq + 3;    /* add remaining terms */
    return result;
}

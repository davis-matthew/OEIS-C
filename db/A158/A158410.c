/*
 * A158410  a(n) = 961*n^2 - 2*n.
 * https://oeis.org/A158410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 961*n^2 - 2*n
 * Range:     u8 none, u16 n <= 8, u32 n <= 2114, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* convert the index to the result type */
    seq_t n_sq = n_t * n_t;        /* n^2 (sequence type handles size) */
    seq_t part = 961 * n_sq;       /* 961 * n^2 */
    seq_t sub  = 2 * n_t;          /* 2 * n */
    return part - sub;             /* the final result */
}

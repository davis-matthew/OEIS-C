/*
 * A224039  Number of 3 X n 0..1 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 6*n^2 + 47*n)/6 for n > 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;

    seq_t n_t = (seq_t)n;
    seq_t term = n_t * n_t * n_t;            /* n^3 */
    term += (seq_t)6 * n_t * n_t;            /* +6*n^2 */
    term += (seq_t)47 * n_t;                 /* +47*n */
    term /= 6;
    return term;
}

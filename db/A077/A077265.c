/*
 * A077265  Number of cycles in the n-th order prism graph.
 * https://oeis.org/A077265
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n*(n-1) (for n ≥ 3)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n < 3 the sequence is not defined; return 0 */
    if (n < 3) {
        return (seq_t)0;
    }

    /* compute 2^n in seq_t, using only + and * operators */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 = pow2 * (seq_t)2;
    }

    /* compute n*(n-1) safely in seq_t */
    seq_t nn = (seq_t)n;
    seq_t term = pow2 + nn * (nn - (seq_t)1);
    return term;
}

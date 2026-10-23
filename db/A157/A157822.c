/*
 * A157822  a(n) = 1482401250*n^2+108900*n+1.
 * https://oeis.org/A157822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1482401250*n^2 + 108900*n + 1
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) is the first term) */
seq_t a(idx_t n)
{
    /* compute the quadratic formula in seq_t to avoid overflow */
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)1482401250u * nn * nn;
    seq_t term2 = (seq_t)108900u * nn;
    return term1 + term2 + (seq_t)1;
}

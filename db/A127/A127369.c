/*
 * A127369  a(n) = (n^3+n)*4^n.
 * https://oeis.org/A127369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3+n)*4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;   /* n^2   */
    seq_t n_cube = n_sq * n_seq;    /* n^3   */
    seq_t poly   = n_cube + n_seq;  /* n^3 + n */

    /* compute 4^n with repeated multiplication */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= 4;

    return poly * pow4;
}

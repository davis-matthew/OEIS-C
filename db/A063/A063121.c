/*
 * A063121  Dimension of the space of weight 2n cusp forms for Gamma_0( 53 ).
 * https://oeis.org/A063121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4; a(n) = 9*n - 5 for n even (n > 1); a(n) = 9*n - 6 for
 *            n odd (n > 1); (This is equivalent to a(n) = ((-1)^n + 18*n -
 *            11)/2 for n > 1.)
 * Range:     u8 n <= 29, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the only exceptional term explicitly */
    if (n == 1u) return 4;
    if (n == 0u) return 0;          /* not part of the OEIS offset,
                                       but safe for out‑of‑range calls */
    /* Even and odd cases for n > 1 */
    if ((n & 1u) == 0u)
        return (seq_t)9 * n - 5;    /* 9*n - 5  (n even) */
    else
        return (seq_t)9 * n - 6;    /* 9*n - 6  (n odd)  */
}

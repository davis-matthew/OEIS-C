/*
 * A219902  Number of isomorphism classes of IPR nanocones with 3 pentagons
 *          and a nearsymmetric boundary of length n.
 * https://oeis.org/A219902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (2*n^3 + 4*n^2 + n)/6 + (n/7 if n even, -ceil(n/3) if n
 *            odd)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = (2 * N * N * N + 4 * N * N + N) / 6;
    if ((n & 1) == 0)          /* n even: add floor(n/7) */
        term += (seq_t)(n / 7);
    else                       /* n odd: subtract ceil(n/3) */
        term -= (seq_t)((n + 2) / 3);
    return term;
}

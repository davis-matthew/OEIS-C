/*
 * A022147  Coordination sequence for root lattice B_5.
 * https://oeis.org/A022147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = (108*n^4 - 88*n^3 + 156*n^2 - 32*n +
 *            6)/3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sn = (seq_t)n;

    /* compute 108*n^4 */
    seq_t term = (seq_t)108;
    term *= sn;   /* 108*n   */
    term *= sn;   /* 108*n^2 */
    term *= sn;   /* 108*n^3 */
    term *= sn;   /* 108*n^4 */

    /* subtract 88*n^3 */
    term -= (seq_t)88 * sn * sn * sn;

    /* add 156*n^2 */
    term += (seq_t)156 * sn * sn;

    /* subtract 32*n */
    term -= (seq_t)32 * sn;

    /* add 6 */
    term += (seq_t)6;

    /* divide by 3 */
    term /= (seq_t)3;

    return term;
}

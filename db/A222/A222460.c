/*
 * A222460  Number of n X 4 0..7 arrays with no entry increasing mod 8 by 7
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1689*a(n-1) - 108775*a(n-2) + 1672631*a(n-3) a(1) = 343,
 *            a(2) = 556549, a(3) = 903055069
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Index 0 is not part of the sequence. */
    if (n == 0) return 0;

    /* Treat the first three values directly. */
    if (n == 1) return (seq_t)343;
    if (n == 2) return (seq_t)556549;
    if (n == 3) return (seq_t)903055069;

    /* Recurrence variables: a1 = a(n-3), a2 = a(n-2), a3 = a(n-1).  */
    seq_t a1 = (seq_t)343;
    seq_t a2 = (seq_t)556549;
    seq_t a3 = (seq_t)903055069;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = (seq_t)1689 * a3
                 - (seq_t)108775 * a2
                 + (seq_t)1672631 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

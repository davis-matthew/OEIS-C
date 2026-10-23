/*
 * A100190  The (4,1)-entry in the 4 X 4 matrix M^n, where M = [1,0,0,0 /
 *          3,3,0,0 / 3,6,3,0 / 1,3,3,1].
 * https://oeis.org/A100190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 22*a(n-2) + 24*a(n-3) - 9*a(n-4) (n ≥ 5);
 *            initial values: a(1)=1, a(2)=20, a(3)=147, a(4)=760
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)147;
    if (n == 4) return (seq_t)760;

    /* Initialise the four previous terms. */
    seq_t a1 = 1, a2 = 20, a3 = 147, a4 = 760;

    /* Iterate up to the requested n. */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = 8 * a4 - 22 * a3 + 24 * a2 - 9 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}

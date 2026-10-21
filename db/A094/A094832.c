/*
 * A094832  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 3, s(2n+1) = 4.
 * https://oeis.org/A094832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + a(n-3); initial values: a(0)=1,
 *            a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)10;
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        /* an = 6*a2 - 9*a1 + a0 */
        an = ((seq_t)6) * a2;
        an -= ((seq_t)9) * a1;
        an += a0;
        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

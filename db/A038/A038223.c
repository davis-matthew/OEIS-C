/*
 * A038223  Bottom line of 3-wave sequence A038196, also bisection of A006356.
 * https://oeis.org/A038223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 5*a(n-2) + a(n-3) for n >= 3 Initial values:
 *            a(0) = 1, a(1) = 6, a(2) = 31
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)31;

    seq_t a0 = (seq_t)1;       /* a(n-3) */
    seq_t a1 = (seq_t)6;       /* a(n-2) */
    seq_t a2 = (seq_t)31;      /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)6 * a2 - (seq_t)5 * a1 + a0;
        a0 = a1;  /* shift window */
        a1 = a2;
        a2 = an;
    }

    return an;
}

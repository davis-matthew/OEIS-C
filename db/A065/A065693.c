/*
 * A065693  Braided power sequence: A065692 is b(n+1) = 3*b(n) + 2*d(n) -
 *          c(n), this is c(n+1) = 3*c(n) + 2*b(n) - d(n) and A065694 is
 *          d(n+1) = 3*d(n) + 2*c(n) - b(n), starting with b(0) = 0, c(0) = 1
 *          and d(0) = 2.
 * https://oeis.org/A065693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 52*a(n-3), with a(0) = 1, a(1) =
 *            1, a(2) = 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1 || n == 2)
        return 1;

    /* Values for a(n-3), a(n-2), a(n-1) */
    seq_t a0 = 1;  /* a(n-3) */
    seq_t a1 = 1;  /* a(n-2) */
    seq_t a2 = 1;  /* a(n-1) */

    /* Iterate up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = a2 * 9 + a0 * 52;   /* 9*a(n-1) + 52*a(n-3) */
        term -= a1 * 33;                  /* - 33*a(n-2)     */
        a0 = a1;
        a1 = a2;
        a2 = term;
    }
    return a2;
}

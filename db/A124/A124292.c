/*
 * A124292  Number of free generators of degree n of symmetric polynomials in
 *          4 noncommuting variables.
 * https://oeis.org/A124292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 3*a(n-3), a(1) = 1, a(2) = 1, a(3)
 *            = 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* below offset, behavior undefined */
    if (n == 1 || n == 2) return 1;
    if (n == 3) return 2;

    seq_t a1 = 1;   /* a(n-3)   */
    seq_t a2 = 1;   /* a(n-2)   */
    seq_t a3 = 2;   /* a(n-1)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 6 * a3 - 9 * a2 + 3 * a1; /* recurrence */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

/*
 * A226700  Solutions y/(3*5*13) of the Pell equation x^2 - 61*y^2 = +4.
 * https://oeis.org/A226700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1523*a(n-1) - a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t a0 = 0;          /* a(n-2) */
    seq_t a1 = 1;          /* a(n-1) */
    seq_t a;               /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        a = ((seq_t)1523) * a1; /* 1523*a(n-1) */
        a -= a0;               /* -a(n-2) */
        a0 = a1;               /* shift a(n-2) */
        a1 = a;                /* shift a(n-1) */
    }
    return a1;
}

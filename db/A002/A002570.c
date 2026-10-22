/*
 * A002570  From a definite integral.
 * https://oeis.org/A002570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + d(n), where d(n) = A002571(n-1) satisfies d(n) =
 *            3*d(n-1) - d(n-2) + (-1)^(n+1)*n for n >= 5, d(3) = 5, d(4) =
 *            10; a(1) = 1, a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t am2 = 1, am1 = 1;          /* a(n-2), a(n-1) */
    seq_t dm2 = 5, dm1 = 10;         /* d(3), d(4) */
    idx_t i;

    if (n <= 2) return 1;
    if (n == 3) return 6;
    if (n == 4) return 11;

    am2 = 6;   /* a(3) */
    am1 = 11;  /* a(4) */
    for (i = 5; i <= n; i++) {
        seq_t d = dm1 * 3;
        if (i & 1) d += (seq_t)i;
        d -= dm2;
        if (!(i & 1)) d -= (seq_t)i;
        seq_t cur = am2 + d;
        dm2 = dm1;
        dm1 = d;
        am2 = am1;
        am1 = cur;
    }
    return am1;
}

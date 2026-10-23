/*
 * A188571  a(n) = coefficient of sqrt(2) in the expansion of (1 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A188571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2)-16*a(n-3)+8*a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=14. This computes the coefficient of
 *            sqrt(2) in (1+sqrt(2)+sqrt(3))^n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 14;

    seq_t a0 = 0;   /* a(n-4) */
    seq_t a1 = 1;   /* a(n-3) */
    seq_t a2 = 2;   /* a(n-2) */
    seq_t a3 = 14;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 + 4 * a2 - 16 * a1 + 8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

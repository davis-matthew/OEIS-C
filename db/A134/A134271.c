/*
 * A134271  a(n) = a(n-2) + 2*a(n-3), n > 3; with a(0)=0, a(1)=1, a(2)=2,
 *          a(3)=0.
 * https://oeis.org/A134271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3); n > 3; Initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=0
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
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
    if (n == 3) return 0;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1), a3 = a(n) */
    seq_t a0 = 0, a1 = 1, a2 = 2, a3 = 0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = a2 + (a1 << 1);   /* a(i) = a(i-2) + 2*a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}

/*
 * A041221  Denominators of continued fraction convergents to sqrt(122).
 * https://oeis.org/A041221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 22*a(n-1) + a(n-2), a(0)=1, a(1)=22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 22;

    seq_t a0 = 1;
    seq_t a1 = 22;
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = 22 * a1 + a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

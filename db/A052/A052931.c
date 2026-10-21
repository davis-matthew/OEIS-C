/*
 * A052931  Expansion of 1/(1 - 3*x^2 - x^3).
 * https://oeis.org/A052931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(1) = 0; a(2) = 3; for n >= 3, a(n) = 3*a(n-2) +
 *            a(n-3);
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)0;
    }
    if (n == 2) {
        return (seq_t)3;
    }

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */
    seq_t a;

    for (idx_t i = 3; i <= n; ++i) {
        a = (seq_t)3 * a1 + a0;   /* a(n) = 3*a(n-2) + a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = a;
    }

    return a2;
}

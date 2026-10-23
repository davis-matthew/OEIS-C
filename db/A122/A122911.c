/*
 * A122911  Expansion of (1+x)*(1-6*x-25*x^2)/((1+2x)(1-4x)(1+8x)(1-16x)).
 * https://oeis.org/A122911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = 10*a(n-1) + 120*a(n-2) - 320*a(n-3) - 1024*a(n-4) *
 *            with initial values * a(0) = 1, a(1) = 5, a(2) = 139, a(3) =
 *            1645. * This recurrence follows from the rational generating
 *            function * (1-5x-31x^2-25x^3)/(1-10x-120x^2+320x^3+1024x^4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)5;
    seq_t a2 = (seq_t)139;
    seq_t a3 = (seq_t)1645;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t = (seq_t)10 * a3 + (seq_t)120 * a2;
        t -= (seq_t)320 * a1;
        t -= (seq_t)1024 * a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = t;
    }
    return a3;
}

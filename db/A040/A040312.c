/*
 * A040312  Continued fraction for sqrt(331).
 * https://oeis.org/A040312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth term of the simple continued fraction expansion of
 *            sqrt(331)
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* D = 331, a0 = floor(sqrt(331)) = 18 */
    static const seq_t D   = 331;
    static const seq_t a0  = 18;

    if (n == 0) return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t an = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next triple (m, d, a) according to standard CF algorithm */
        m = d * an - m;
        d = (D - m * m) / d;
        an = (a0 + m) / d;
    }
    return an;
}

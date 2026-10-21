/*
 * A063230  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          71 ).
 * https://oeis.org/A063230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) for n >= 7; a(1) = 6, a(2) = 17,
 *            a(3) = 29, a(4) = 41, a(5) = 53, a(6) = 63
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 1..6 */
    static const seq_t base[6] = {6, 17, 29, 41, 53, 63};

    if (n <= (idx_t)6)
        return base[n - 1];

    /* Sliding window for the last five terms:
       x0 = a(n-5), x1 = a(n-4), x2 = a(n-3), x3 = a(n-2), x4 = a(n-1) */
    seq_t x0 = base[1]; /* a2 */
    seq_t x1 = base[2]; /* a3 */
    seq_t x2 = base[3]; /* a4 */
    seq_t x3 = base[4]; /* a5 */
    seq_t x4 = base[5]; /* a6 */
    seq_t ai = 0;

    for (idx_t i = 7; i <= n; ++i) {
        ai = x3 + x2 - x0;          /* a(i) = a(i-2) + a(i-3) - a(i-5) */
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = ai;
    }

    return ai;
}

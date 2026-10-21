/*
 * A059990  Number of points of period n under the dual of the map x->2x on
 *          Z[1/6].
 * https://oeis.org/A059990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1) / 3^{v_3(2^n-1)}, where v_3(m) is the exponent
 *            of 3 dividing m
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For even n, v_3(2^n-1) = v_3(n) + 1; for odd n it is 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* not used, but safe */

    /* compute 2^n - 1 */
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        val = val * (seq_t)2;
    val = val - (seq_t)1;

    /* determine exponent of 3 dividing 2^n-1 */
    seq_t divisor = (seq_t)1;
    if ((n & 1) == 0) {                         /* n even */
        idx_t t = n;
        idx_t v3n = 0;
        while ((t % (idx_t)3) == 0) {
            ++v3n;
            t /= (idx_t)3;
        }
        idx_t exp = v3n + 1;                   /* v_3(2^n-1) */
        for (idx_t i = 0; i < exp; ++i)
            divisor = divisor * (seq_t)3;
    }

    /* divide by the power of 3 */
    val = val / divisor;
    return val;
}

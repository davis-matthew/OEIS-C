/*
 * A200407  The x-values in the solution to 19*x^2 - 18 = y^2.
 * https://oeis.org/A200407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 340*a(n-3) - a(n-6) for n ≥ 7, with initial terms a(1)=1,
 *            a(2)=9, a(3)=131, a(4)=209, a(5)=3051, a(6)=44539
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n < 1; guard */

    /* initial terms */
    static const seq_t init[6] = {1, 9, 131, 209, 3051, 44539};

    if (n <= 6)
        return init[n - 1];

    /* circular buffer for the last six values */
    seq_t buf[6];
    buf[0] = init[0]; /* a(1) */
    buf[1] = init[1]; /* a(2) */
    buf[2] = init[2]; /* a(3) */
    buf[3] = init[3]; /* a(4) */
    buf[4] = init[4]; /* a(5) */
    buf[5] = init[5]; /* a(6) */

    for (idx_t i = 7; i <= n; ++i)
    {
        idx_t idx_a   = (i - 3 - 1) % 6;          /* a(i-3) */
        idx_t idx_b   = (i - 6 - 1) % 6;          /* a(i-6) */
        idx_t idx_new = (i - 1) % 6;              /* position for a(i) */

        seq_t val = (seq_t)340 * buf[idx_a] - buf[idx_b];
        buf[idx_new] = val;
    }

    return buf[(n - 1) % 6];
}

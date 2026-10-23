/*
 * A042485  Denominators of continued fraction convergents to sqrt(770).
 * https://oeis.org/A042485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 222*a(n-4) - a(n-8) with initial values
   a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 4,
   a(4) = 219, a(5) = 223, a(6) = 665, a(7) = 888. */

seq_t a(idx_t n)
{
    /* constant initial terms */
    static const seq_t init[8] = {1, 1, 3, 4, 219, 223, 665, 888};

    if (n < 8)
        return init[n];

    /* Circular buffer holding last 8 computed terms */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t a_n_4 = buf[(i - 4) & 7];
        seq_t a_n_8 = buf[(i - 8) & 7];
        buf[i & 7] = (seq_t)222 * a_n_4 - a_n_8;
    }

    return buf[n & 7];
}

/*
 * A042043  Denominators of continued fraction convergents to sqrt(545).
 * https://oeis.org/A042043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3922*a(n-6) - a(n-12) for n > 11 Initial values: a(0) =
 *            1, a(1) = 2, a(2) = 3, a(3) = 26, a(4) = 29, a(5) = 84, a(6) =
 *            3893, a(7) = 7870, a(8) = 11763, a(9) = 101974, a(10) = 113737,
 *            a(11) = 329448
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 12‑term recurrence: only need the last 12 values */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)26,
        (seq_t)29, (seq_t)84, (seq_t)3893, (seq_t)7870,
        (seq_t)11763, (seq_t)101974, (seq_t)113737, (seq_t)329448
    };

    if (n < 12) {
        return init[n];
    }

    /* Buffer for the 12 most recent terms */
    seq_t buf[12];
    idx_t i;

    /* Copy initial values into buffer */
    for (i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute up to n using the recurrence */
    for (i = 12; i <= n; ++i) {
        seq_t val = 3922 * buf[(i - 6) % 12] - buf[(i - 12) % 12];
        buf[i % 12] = val;
    }

    return buf[n % 12];
}

/*
 * A014341  Four-fold exponential convolution of Fibonacci numbers with
 *          themselves (divided by 24).
 * https://oeis.org/A014341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) – 15*a(n-2) – 70*a(n-3) + 120*a(n-4) +
 *            32*a(n-5) (n ≥ 5) Initial values: a(0)=0, a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=1. This follows from the rational generating
 *            function G(x) = x⁴ / (1 – 10x – 15x² + 70x³ + 120x⁴ + 32x⁵)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 4 */
    static const seq_t init[5] = {0, 0, 0, 0, 1};

    if (n < 5) {
        return init[n];
    }

    /* circular buffer holding a(n-5) … a(n-1) */
    seq_t t[5] = {init[0], init[1], init[2], init[3], init[4]};

    seq_t pos, sub, val;
    for (idx_t i = 5; i <= n; ++i) {
        /* compute 10*a(n-1) + 120*a(n-4) + 32*a(n-5) */
        pos = 10 * t[4] + 120 * t[1] + 32 * t[0];
        /* compute 15*a(n-2) + 70*a(n-3) */
        sub = 15 * t[3] + 70 * t[2];
        /* a(n) = pos - sub  (pos ≥ sub for all n≥5) */
        val = pos - sub;

        /* shift buffer for next iteration */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = t[4];
        t[4] = val;
    }

    return t[4];
}

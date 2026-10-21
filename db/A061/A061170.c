/*
 * A061170  Fourth column of Lucas bisection triangle (even part).
 * https://oeis.org/A061170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 58*a(n-2) + 144*a(n-3) - 195*a(n-4) +
 *            144*a(n-5) - 58*a(n-6) + 12*a(n-7) - a(n-8), with p(n) =
 *            {1,58,123,-278,193,-72,16} for n = 0..6 (the generating‑function
 *            numerator gives the inhomogeneous terms p(n) for n ≤ 6)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 32, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The numerator coefficients of the generating function
       (1+58*x+123*x^2-278*x^3+193*x^4-72*x^5+16*x^6).     */
    static const int64_t p[7] = { 1, 58, 123, -278, 193, -72, 16 };

    /* ring[i] holds a[n-8+i] for the current value of n.
       All entries are initialized to zero for n < 0. */
    seq_t ring[8] = { 0 };
    /*** iterate from 0 up to the requested index ***/
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t val = 0;                /* value for a[i] */

        /* --- positive contributions --- */
        if (i <= 6 && p[i] > 0)
            val += (seq_t)p[i];

        if (i >= 1)
            val += (seq_t)12 * ring[(i - 1) & 7];

        if (i >= 3)
            val += (seq_t)144 * ring[(i - 3) & 7];

        if (i >= 5)
            val += (seq_t)144 * ring[(i - 5) & 7];

        if (i >= 7)
            val += (seq_t)12 * ring[(i - 7) & 7];

        /* --- negative contributions --- */
        if (i <= 6 && p[i] < 0)
            val -= (seq_t)(-p[i]);     /* subtract |p[i]| */

        if (i >= 2)
            val -= (seq_t)58 * ring[(i - 2) & 7];

        if (i >= 4)
            val -= (seq_t)195 * ring[(i - 4) & 7];

        if (i >= 6)
            val -= (seq_t)58 * ring[(i - 6) & 7];

        if (i >= 8)
            val -= ring[(i - 8) & 7];

        /* store computed value for future iterations */
        ring[i & 7] = val;
    }
    return ring[n & 7];
}

/*
 * A116812  Number of permutations of length n which avoid the patterns 2314,
 *          3421, 4123.
 * https://oeis.org/A116812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = p(n) + 10*a(n-1) - 43*a(n-2) + 106*a(n-3)
 *               - 167*a(n-4) + 174*a(n-5) - 118*a(n-6)
 *               + 48*a(n-7) - 9*a(n-8)
 *
 * where p(n) is the coefficient of x^n in the numerator of the
 * generating function:
 *   N(x) = x*(x^9 - 2x^8 -10x^7 +35x^6 -64x^5 +75x^4 -59x^3 +29x^2 -8x + 1)
 *
 *   p(0)=0, p(1)=1, p(2)=-8, p(3)=29, p(4)=-59, p(5)=75,
 *   p(6)=-64, p(7)=35, p(8)=-10, p(9)=-2, p(10)=1.
 *
 * Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *                 a(5)=72, a(6)=229, a(7)=683.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used, but safe */

    /* p(n) for 0 ≤ n ≤ 10 */
    static const long long P[11] = {
        0, 1, -8, 29, -59, 75, -64, 35, -10, -2, 1
    };

    /* we only need the last 8 values – a rolling buffer of size 9 suffices */
    seq_t buf[9] = {0};

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = 0;

        /* add the constant term p(i) */
        if (i <= 10) {
            if (P[i] >= 0)
                term += (seq_t)P[i];
            else
                term -= (seq_t)(-P[i]);   /* subtract the positive magnitude */
        }

        /* recurrence terms (indices are wrapped with % 9) */
        if (i >= 1) term += (seq_t)10 * buf[(i - 1) % 9];
        if (i >= 2) term -= (seq_t)43 * buf[(i - 2) % 9];
        if (i >= 3) term += (seq_t)106 * buf[(i - 3) % 9];
        if (i >= 4) term -= (seq_t)167 * buf[(i - 4) % 9];
        if (i >= 5) term += (seq_t)174 * buf[(i - 5) % 9];
        if (i >= 6) term -= (seq_t)118 * buf[(i - 6) % 9];
        if (i >= 7) term += (seq_t)48 * buf[(i - 7) % 9];
        if (i >= 8) term -= (seq_t)9 * buf[(i - 8) % 9];

        buf[i % 9] = term;
    }

    return buf[n % 9];
}

/*
 * A105369  Expansion of ((1+x)^3 - x^3)/((1+x)^5 - x^5).
 * https://oeis.org/A105369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 0 for n < 0 a(0) = 1, a(1) = -2, a(2) = 3, a(3) = -5
 *            Recurrence for n ≥ 4: a(n) = -5·a(n‑1) -10·a(n‑2) -10·a(n‑3)
 *            -5·a(n‑4) This follows from the generating function (1 + 3x +
 *            3x²) / (1 + 5x + 10x² + 10x³ + 5x⁴) which gives the linear
 *            recurrence with the stated coefficients
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 33, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values: a[0] .. a[3] */
    static const seq_t init[4] = { (seq_t)1, (seq_t)(-2),
                                   (seq_t)3, (seq_t)(-5) };
    if (n < 4) return init[n];

    /* Variables hold the four preceding terms:
       a_nm4 = a(n‑4), a_nm3 = a(n‑3), a_nm2 = a(n‑2), a_nm1 = a(n‑1) */
    seq_t a_nm4 = init[0];
    seq_t a_nm3 = init[1];
    seq_t a_nm2 = init[2];
    seq_t a_nm1 = init[3];
    seq_t a_n   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a_n = -5*a_nm1 -10*a_nm2 -10*a_nm3 -5*a_nm4 */
        a_n = (seq_t)(-5) * a_nm1
            + (seq_t)(-10) * a_nm2
            + (seq_t)(-10) * a_nm3
            + (seq_t)(-5) * a_nm4;

        /* Shift the window of previous terms */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

/*
 * A028192  Expansion of 1/((1-5*x)*(1-8*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A028192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 34 Recurrence: a(n) = 21*a(n-1) - 108*a(n-2) +
 *            (8^(n+1) - 5^(n+1))/3 (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;

    /* a0 = a(n-2), a1 = a(n-1) */
    seq_t a0 = 1;
    seq_t a1 = 34;
    seq_t ai = 0;

    /* Powers for the non‑homogeneous term
       For the first iteration (n = 2) we need 8^(3) and 5^(3). */
    seq_t pow8 = 512;   /* 8^3 */
    seq_t pow5 = 125;   /* 5^3 */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute (8^(i+1) - 5^(i+1)) / 3  */
        seq_t term = (pow8 - pow5) / (seq_t)3;

        /* a[i] = 21*a1 - 108*a0 + term   */
        ai = 21 * a1 - 108 * a0 + term;

        /* Shift for the next step */
        a0 = a1;
        a1 = ai;

        /* Update the powers for the next recurrence step */
        pow8 *= (seq_t)8;
        pow5 *= (seq_t)5;
    }

    return ai;
}

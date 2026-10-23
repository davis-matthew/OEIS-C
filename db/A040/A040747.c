/*
 * A040747  Continued fraction for sqrt(775).
 * https://oeis.org/A040747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(775).  Length of the period is 12:
   a(0) = floor(sqrt(775)) = 27.
   The terms a(1),a(2),… are the coefficients of the periodic
   continued‑fraction expansion of sqrt(775). */
seq_t a(idx_t n)
{
    /* Constant for the quadratic surd D = 775 */
    const idx_t D   = 775U;
    const idx_t a0  = 27U;          /* floor(sqrt(D)) */

    /* Handle the offset */
    if (n == 0)
        return (seq_t)a0;

    /* Cache the period.  The period has length 12 for sqrt(775). */
    static seq_t period[128];
    static idx_t period_len = 0;

    if (period_len == 0) {
        /* Compute the period of the continued fraction once. */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;                 /* a will hold the previous a */

        while (1) {
            /* Standard algorithm for continued fractions of sqrt(D) */
            m = d * a - m;            /* new m */
            d = (D - m * m) / d;      /* new d */
            a = (a0 + m) / d;         /* new a */

            period[period_len++] = (seq_t)a;

            if (a == 2 * a0)          /* last term of the period */
                break;
        }
    }

    /* a(n) for n >= 1 is period[(n-1) mod period_len] */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

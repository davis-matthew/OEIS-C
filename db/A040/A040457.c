/*
 * A040457  Continued fraction for sqrt(479).
 * https://oeis.org/A040457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040457: Continued fraction for sqrt(479) */

seq_t a(idx_t n)
{
    /* Pre‑compute the periodic part of the continued fraction for sqrt(479). */
    static seq_t period[20];
    static idx_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        const idx_t D   = 479;
        const idx_t a0  = 21;   /* floor(sqrt(479)) */
        idx_t m = 0, d = 1, a = a0;

        period_len = 0;
        do {
            /* Standard recurrence for continued fractions of quadratic surds. */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0);   /* 2*a0 marks the end of the period */

        initialized = 1;
    }

    if (n == 0)
        return (seq_t)21;                 /* a(0) = floor(sqrt(479)) */

    idx_t k = (n - 1) % period_len;       /* periodic index: a(n) = period[(n-1)%L] */
    return period[k];
}

/*
 * A040382  Continued fraction for sqrt(403).
 * https://oeis.org/A040382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the nth term of the continued fraction expansion of
 *            sqrt(403)
 * Range:     u8 none, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The expansion of sqrt(403) is periodic.  Pre‑compute its period once. */
    static seq_t terms[12];       /* a0 followed by the periodic block */
    static unsigned int period = 0;
    static int initialized = 0;

    if (!initialized) {
        const seq_t D = 403;
        const seq_t a0 = 20;      /* floor(sqrt(403)) */

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        terms[0] = a0;
        unsigned int idx = 1;

        while (1) {
            /* next values in the continued fraction */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            terms[idx++] = a;
            if (a == (seq_t)(2 * a0))
                break;               /* end of the period */
        }
        period = idx - 1;            /* number of terms in the periodic block */
        initialized = 1;
    }

    if (n == 0) {
        return terms[0];
    } else {
        idx_t p = (idx_t)period;     /* small period fits in idx_t */
        idx_t pos = ((n - 1) % p) + 1;
        return terms[(unsigned int)pos];
    }
}

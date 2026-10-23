/*
 * A040911  Continued fraction for sqrt(942).
 * https://oeis.org/A040911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction term of sqrt(942)
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(942)) = 30
   For n > 0: a(n) repeats the period [1,2,4,20,4,2,1,60] */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t sqrtD = 0;
    static seq_t pattern[20];
    static size_t period_len = 0;

    if (!initialized) {
        /* compute floor(sqrt(942)) */
        for (sqrtD = 0;
             (sqrtD + 1) * (sqrtD + 1) <= (seq_t)942;
             ++sqrtD)
            ;

        /* generate periodic part of continued fraction for sqrt(942) */
        const seq_t D = 942;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = sqrtD;

        while (1) {
            /* next terms per standard continued fraction algorithm */
            m = d * a_term - m;
            d = (D - m * m) / d;
            a_term = (sqrtD + m) / d;

            if (period_len >= sizeof(pattern) / sizeof(pattern[0]))
                break; /* safety */

            pattern[period_len++] = a_term;

            if (a_term == 2 * sqrtD)
                break; /* period completed */
        }
        initialized = 1;
    }

    if (n == 0)
        return sqrtD;

    /* index into the period (period_len == 8 for sqrt(942)) */
    idx_t idx = (n - 1) % period_len;
    return pattern[idx];
}

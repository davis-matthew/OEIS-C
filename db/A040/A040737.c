/*
 * A040737  Continued fraction for sqrt(765).
 * https://oeis.org/A040737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040737: coefficients of the continued fraction expansion of √765 */
#include <stdbool.h>

/* Compute a(n) for the continued fraction of sqrt(765) */
seq_t a(idx_t n)
{
    /* The integer part of √765 */
    const seq_t a0 = 27;
    /* Periodic part after the initial term */
    static seq_t period[100];
    static idx_t period_len = 0;
    static bool initialized = false;

    if (!initialized)
    {
        /* Build the period by the standard continued‑fraction algorithm.
           It terminates when the denominator becomes 1, at which point the
           next term would equal 2*a0 and the sequence repeats. */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        idx_t i = 0;
        do
        {
            /* Compute next triple (m, d, a) */
            m = d * a - m;
            /* (D - m*m) is divisible by d; D = 765 */
            d = ((seq_t)765 - m * m) / d;
            a = (a0 + m) / d;

            period[i++] = a;
        } while (d != 1);   /* d==1 signals the end of the period */

        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return a0;  /* a(0) is the initial term */

    /* After the first term the sequence repeats with period_len. */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

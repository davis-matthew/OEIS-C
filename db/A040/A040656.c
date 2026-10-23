/*
 * A040656  Continued fraction for sqrt(683).
 * https://oeis.org/A040656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) are the continued‑fraction coefficients of √683
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Period length of the continued fraction of √683 */
    enum { PERIOD = 14 };

    static bool init    = false;
    static seq_t period[PERIOD];
    static seq_t a0;   /* floor(sqrt(683)) */

    if (!init) {
        init = true;
        a0   = 26;          /* ⌊√683⌋ = 26 */

        const seq_t N = 683;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        size_t idx = 0;

        /* Generate terms until we hit the terminating term 2*a0 */
        while (1) {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;          /* floor division */
            period[idx++] = a;
            if (a == 2 * a0)            /* terminating condition */
                break;
        }
    }

    if (n == 0)
        return a0;

    /* The sequence repeats with period PERIOD after the first term */
    idx_t pos = (n - 1) % PERIOD;
    return period[(size_t)pos];
}

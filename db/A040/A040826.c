/*
 * A040826  Continued fraction for sqrt(856).
 * https://oeis.org/A040826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: continued fraction coefficients of sqrt(856) via
   the standard recurrence for quadratic irrationals. */

#include <stdbool.h>
#include <stddef.h>

static const idx_t            D   = 856ULL;           /* radicand */
static const seq_t            A0  = (seq_t)29;         /* floor(sqrt(D)) */

seq_t a(idx_t n)
{
    /* The continued‐fraction expansion of sqrt(D) is periodic.
       We compute the period once and reuse it for all calls.   */
    static bool              ready        = false;
    static seq_t             period[2000]; /* sufficient for D=856 */
    static size_t            period_len   = 0;

    if (!ready) {
        const idx_t a0 = (idx_t)A0;   /* integer part of sqrt(D)   */
        idx_t m   = 0;
        idx_t d   = 1;
        idx_t a_cur = a0;
        size_t len = 0;

        while (1) {
            /* Standard recurrence for continued fractions of sqrt(D). */
            m      = d * a_cur - m;
            d      = (D - m * m) / d;
            a_cur  = (a0 + m) / d;

            period[len++] = (seq_t)a_cur;
            if (a_cur == 2 * a0)               /* end of period */
                break;
        }
        period_len = len;
        ready = true;
    }

    if (n == 0)
        return A0;

    size_t pos = (size_t)(n - 1) % period_len;
    return period[pos];
}

/*
 * A040821  Continued fraction for sqrt(851).
 * https://oeis.org/A040821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n-th term of the continued fraction expansion of
 *            sqrt(851) (a(0) = ⌊√851⌋, a(1..) repeat every period)
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static variables to store the period of the continued fraction */
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static seq_t a0 = 0;
    static int initialized = 0;

    if (!initialized) {
        /* compute a0 = floor(sqrt(851)) */
        a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= 851) {
            a0 = a0 + 1;
        }

        /* build the period using the standard recurrence for sqrt(N) */
        const seq_t N = 851;
        size_t cap = 32;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t len = 0;

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;
        while (1) {
            /* next m, d, a */
            m = d * a_cur - m;
            d = (N - m * m) / d;
            a_cur = (a0 + m) / d;

            /* store current a_cur (this is the next continued‑fraction term) */
            if (len >= cap) {
                cap *= 2;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[len++] = a_cur;

            /* stop when we hit the terminating term 2*a0 */
            if (a_cur == 2 * a0) {
                break;
            }
        }
        period_len = len;
        initialized = 1;
    }

    if (n == 0) {
        return a0;
    } else {
        idx_t pos = (n - 1) % period_len;
        return period[pos];
    }
}

/*
 * A040964  Continued fraction for sqrt(996).
 * https://oeis.org/A040964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   returns the n‑th term of the continued fraction expansion of
 *            sqrt(996). The sequence starts with a(0)=floor(sqrt(996))=31,
 *            and for n>0 it yields the periodic continued fraction terms
 * Range:     u8 none, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Initial term */
    seq_t const D = 996;
    seq_t const a0 = floor_sqrt(D);

    if (n == 0) return a0;

    /* Periodic part is computed once and cached */
    static seq_t *period = NULL;
    static idx_t period_len = 0;
    if (period == NULL) {
        /* Estimate an initial capacity; the period for sqrt(996) is short. */
        idx_t cap = 64;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (period == NULL) return 0; /* allocation failure */

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_prev = a0;
        idx_t len = 0;

        while (1) {
            /* m = d * a_prev - m */
            m = d * a_prev - m;
            /* d = (D - m*m) / d */
            d = (D - m * m) / d;
            /* a = floor((a0 + m) / d) */
            seq_t a_cur = (a0 + m) / d;

            /* Grow the storage if needed */
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (tmp == NULL) { free(period); return 0; }
                period = tmp;
            }
            period[len++] = a_cur;

            /* The period ends when a_cur == 2*a0 */
            if (a_cur == 2 * a0) {
                period_len = len;
                break;
            }
            a_prev = a_cur;
        }
    }

    /* Return the n-th term using the cached period. */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

/*
 * A041532  Numerators of continued fraction convergents to sqrt(283).
 * https://oeis.org/A041532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Recurrence for the numerators of the continued‑fraction
 *            convergents of sqrt(283). a(0) = 16. For n ≥ 1: a(n) =
 *            period[(n‑1) % 18] * a(n‑1) + a(n‑2), where period =
 *            {1,4,1,1,1,3,10,1,15,1,10,3,1,1,1,4,1,32}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

enum { PERIOD_LEN = 18 };
static const unsigned int period[PERIOD_LEN] = {
    1,4,1,1,1,3,10,1,15,1,10,3,1,1,1,4,1,32
};

seq_t a(idx_t n)
{
    seq_t prevprev = (seq_t)0; /* p[-2] */
    seq_t prev     = (seq_t)1; /* p[-1] */
    seq_t cur      = 0;
    for (idx_t i = 0; i <= n; ++i) {
        unsigned int ai;
        if (i == 0) {
            ai = 16;               /* a0 = floor(sqrt(283)) */
        } else {
            ai = period[(size_t)(i - 1) % PERIOD_LEN];
        }
        cur = ((seq_t)ai) * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}

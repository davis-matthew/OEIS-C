/*
 * A040937  Continued fraction for sqrt(969).
 * https://oeis.org/A040937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction partial quotients for sqrt(969).
   a(0) = floor(sqrt(969)) = 31.
   For n >= 1, a(n) is the (n-1)-th term of the repeating period
   which ends when the continued‑fraction term equals 2*a(0).  */

/* Include the integer square‑root utility */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* D is the radicand; a0 is the integer part of the square root */
    static const seq_t D = 969;
    static seq_t a0 = 0;               /* will be initialized once */
    static seq_t period[64];           /* period length <= 2*a0 = 62 */
    static idx_t period_len = 0;

    /* One‑time initialization of a0 and the period */
    if (period_len == 0) {
        a0 = floor_sqrt(D);        /* a0 = 31 */

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        do {
            /* Compute the next tuple (m, d, a) */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;   /* record the periodic part */
        } while (a != 2 * a0);
    }

    if (n == 0)
        return a0;

    /* Periodic part repeats indefinitely */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

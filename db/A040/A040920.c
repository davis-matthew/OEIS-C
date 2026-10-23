/*
 * A040920  Continued fraction for sqrt(951).
 * https://oeis.org/A040920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) = continued fraction coefficient of √951 at index n
   (OEIS A040920). The sequence starts with a(0)=30 and has a periodic
   part of length 18: [1,5,5,2,3,1,1,1,9,1,1,1,3,2,5,5,1,60].
   The function computes the period only once and then returns the
   appropriate coefficient for any n ≥ 0. */

#include <stddef.h>
#include <stdbool.h>

static seq_t period[32];
static size_t period_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Initialize the periodic part of the continued fraction for √951 */
    if (!initialized) {
        const idx_t D = 951;          /* quadratic radicand */
        const idx_t a0 = 30;          /* floor(√951) */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        period_len = 0;
        while (true) {
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = (seq_t)a_next;
            if (a_next == 2 * a0)   /* end of period */
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }
        initialized = true;
    }

    if (n == 0)        /* a(0) = 30 */
        return 30;

    /* Return the periodic term: a(n) = period[(n-1) mod period_len] */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}

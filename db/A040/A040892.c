/*
 * A040892  Continued fraction for sqrt(923).
 * https://oeis.org/A040892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Continued fraction expansion of sqrt(923).  \
   a(0) = floor(sqrt(923)).  For n ≥ 1 the sequence is periodic \
   with period [2,1,1,1,2,60] (length 6).  The first call computes \
   the period; subsequent calls use the cached values. */
#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0 = 0;               /* floor(sqrt(923)) */
    static seq_t period[64];          /* buffer for the period (enough for any D ≤ 2^32) */
    static size_t period_len = 0;

    if (!initialized) {
        const seq_t D = 923;
        a0 = floor_sqrt(D);

        /* Compute the period of the continued fraction for sqrt(D).
           The period ends when a_next equals 2*a0.            */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;
        size_t i = 0;

        while (1) {
            seq_t m_next = d * a_curr - m;

            /* Compute d_next = (D - m_next^2) / d.
               Use a wider type for the square to avoid overflow
               when seq_t is small.                         */
            unsigned long long m_next_sq = (unsigned long long)m_next * (unsigned long long)m_next;
            seq_t d_next = (seq_t)((D - (seq_t)m_next_sq) / d);

            seq_t a_next = (seq_t)((a0 + m_next) / d_next);

            period[i++] = a_next;
            if (a_next == 2 * a0) {   /* period detected */
                period_len = i;
                break;
            }

            m = m_next;
            d = d_next;
            a_curr = a_next;
        }
        initialized = true;
    }

    if (n == 0) {
        return a0;   /* a(0) */
    }

    /* For n ≥ 1, return the appropriate periodic term. */
    size_t pos = (size_t)((n - 1) % period_len);
    return period[pos];
}

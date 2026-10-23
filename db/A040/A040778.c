/*
 * A040778  Continued fraction for sqrt(807).
 * https://oeis.org/A040778
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

/* Continued fraction terms of sqrt(807). 
   a(0) = floor(sqrt(807)) = 28. 
   For n >= 1, a(n) is given by the standard continued fraction
   recurrence for quadratic irrationals:
      m0 = 0, d0 = 1, a0 = floor(sqrt(807))
      mk+1 = dk * ak - mk
      dk+1 = (807 - mk+1^2) / dk
      ak+1 = floor((a0 + mk+1) / dk+1)
   The period ends when ak+1 == 2*a0 = 56, the last term of the period. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static bool initialized = false;
static seq_t a0;
static seq_t period[128];
static size_t period_len;

/* Compute the period of the continued fraction expansion once. */
static void compute_period(void) {
    const seq_t N = 807;
    seq_t m = 0;
    seq_t d = 1;
    a0 = floor_sqrt(N);
    seq_t twice_a0 = a0 + a0;   /* 2*a0 = 56 */
    period_len = 0;

    /* current a term (a0 initially, but not part of period) */
    seq_t a_cur = a0;

    for (;;) {
        /* compute next m, d, a */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        period[period_len++] = a_next;  /* store term of the period */

        if (a_next == twice_a0) {
            /* period finished, include the last term (56) */
            break;
        }
        /* advance to next iteration */
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
}

/* Return the n-th term of the continued fraction for sqrt(807). */
seq_t a(idx_t n) {
    if (!initialized) {
        compute_period();
        initialized = true;
    }
    if (n == 0) {
        return a0;
    }
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}

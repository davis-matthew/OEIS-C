/*
 * A040522  Continued fraction for sqrt(546).
 * https://oeis.org/A040522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n-th coefficient in the continued fraction expansion
 *            of sqrt(546)
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Computes a(n), the n-th term of the continued fraction for sqrt(546). */
seq_t a(idx_t n)
{
    /* static data for the periodic part after the initial term */
    static bool initialized = false;
    static seq_t a0_val;         /* floor(sqrt(546)) = 23 */
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static size_t period_cap = 0;

    if (!initialized) {
        /* compute a0_val by simple integer search */
        a0_val = 0;
        seq_t D = (seq_t)546;
        while ((a0_val + 1) * (a0_val + 1) <= D) {
            a0_val++;
        }

        /* generate the periodic coefficients until 2*a0_val appears */
        seq_t m = 0;                /* initial m0 */
        seq_t d = 1;                /* initial d0 */
        seq_t a_val = a0_val;       /* initial a0 not stored in period */

        /* dynamic array for period coefficients */
        period_cap = 8;                           /* initial capacity */
        period = (seq_t *)malloc(period_cap * sizeof(seq_t));
        if (!period) {
            /* memory allocation failed; return 0 as fallback */
            return (seq_t)0;
        }

        do {
            /* next triple (m_k, d_k, a_k) */
            m = d * a_val - m;
            seq_t temp = D - m * m;
            d = temp / d;
            a_val = (a0_val + m) / d;

            /* store a_k in period */
            if (period_len == period_cap) {
                period_cap *= 2;
                period = (seq_t *)realloc(period, period_cap * sizeof(seq_t));
                if (!period) {
                    return (seq_t)0;
                }
            }
            period[period_len++] = a_val;
        } while (a_val != 2 * a0_val);

        initialized = true;
    }

    if (n == 0) {
        return a0_val;
    } else {
        /* period starts from a(1); use modulo period_len */
        size_t idx = (size_t)(n - 1) % period_len;
        return period[idx];
    }
}

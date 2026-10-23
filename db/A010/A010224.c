/*
 * A010224  Continued fraction for sqrt(181).
 * https://oeis.org/A010224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n-th term of the continued fraction expansion of
 *            sqrt(181) (OEIS A010224). The expansion is periodic with period
 *            length 21, so a(0) = floor(sqrt(181)) = 13, and for n > 0 a(n) =
 *            period[ (n-1) mod period_len ]. The period is generated once on
 *            the first call
 * Range:     u8 none, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include <stdbool.h>
#include <stddef.h>

/* Period buffer: 200 entries is more than enough for sqrt(181). */
static seq_t period[200];
static idx_t period_len = 0;
static seq_t a0_val = 0;      /* a(0) */
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Compute the continued fraction coefficients of sqrt(181) once. */
        seq_t D   = (seq_t)181;
        a0_val    = floor_sqrt(D);          /* a(0) */
        seq_t m   = 0;
        seq_t d   = 1;
        seq_t a    = a0_val;                /* current a term */
        /* Generate the periodic part (including the final 2*a0). */
        period_len = 0;
        do {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a - m;
            /* d_{k+1} = (D - m_{k+1}^2) / d_k */
            d = (D - m * m) / d;
            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
            a = (a0_val + m) / d;
            period[period_len++] = a;
        } while (a != (a0_val + a0_val)); /* stop when a == 2*a0 */
        initialized = true;
    }

    if (n == 0)
        return a0_val;

    /* For n > 0, return the (n-1)-th entry of the period. */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}

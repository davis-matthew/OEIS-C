/*
 * A041898  Numerators of continued fraction convergents to sqrt(471).
 * https://oeis.org/A041898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 18, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes a(n) : the numerator of the n-th convergent to sqrt(471).
   Continued‑fraction of sqrt(471) is
     [21;(1,2,2,1,3,4,14,4,3,1,2,2,1,42)] .
   Let p[-1] = 1, p[0] = 21.  For k ≥ 1
       p[k] = a_k * p[k-1] + p[k-2] ,
   where a_k is the k‑th periodic coefficient (period length 14).
   The function returns p[n]. */

static const int period[14] = {1,2,2,1,3,4,14,4,3,1,2,2,1,42};
static const size_t PERIOD_LEN = 14;

seq_t a(idx_t n)
{
    /* The first convergent corresponds to a0 = floor(sqrt(471)) = 21 */
    if (n == 0)
        return (seq_t)21;

    seq_t p_prev  = (seq_t)21;   /* p[0] */
    seq_t p_prev2 = (seq_t)1;    /* p[-1] */

    for (idx_t i = 1; i <= n; ++i)
    {
        size_t idx = (size_t)(i - 1) % PERIOD_LEN;
        int ai = period[idx];
        seq_t p_cur = (seq_t)ai * p_prev + p_prev2;
        p_prev2 = p_prev;
        p_prev  = p_cur;
    }

    return p_prev;
}

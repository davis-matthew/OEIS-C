/*
 * A040628  Continued fraction for sqrt(654).
 * https://oeis.org/A040628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = nth term of the continued fraction expansion of sqrt(654)
 *            * a(0) = 25 * For n ≥ 1 the period repeats
 * Range:     u8 n <= 0, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t a0 = (seq_t)25;          /* Integer part of sqrt(654)            */
    static seq_t *period = NULL;          /* Periodic terms after a0              */
    static size_t period_len = 0;         /* Number of terms in the period        */
    static size_t period_cap = 0;         /* Allocated capacity of the array      */

    if (n == 0) return a0;                /* first term */

    /* Compute the period only once */
    if (period_len == 0) {
        const idx_t D = 654;              /* Square‑free part                   */
        seq_t m = (seq_t)0;               /* m_0                                 */
        seq_t d = (seq_t)1;               /* d_0                                 */
        seq_t a = a0;                     /* a_0                                 */

        /* The period ends when a = 2*a0           */
        do {
            /* m_{k+1} = d_k * a_k - m_k           */
            m = d * a - m;

            /* d_{k+1} = (D - m_{k+1}^2) / d_k     */
            d = ((seq_t)D - m * m) / d;

            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})  */
            a = (a0 + m) / d;

            /* Store a in the periodic array */
            if (period_len == period_cap) {
                period_cap = period_cap ? period_cap * 2 : 4;
                period = (seq_t*)realloc(period,
                                         period_cap * sizeof(seq_t));
            }
            period[period_len++] = a;
        } while (a != 2 * a0);            /* End when the standard symmetry occurs */
    }

    /* Index into the periodic part (n-1) */
    idx_t idx_n = (n - (idx_t)1) % (idx_t)period_len;
    return period[(size_t)idx_n];
}

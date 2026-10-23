/*
 * A041989  Denominators of continued fraction convergents to sqrt(517).
 * https://oeis.org/A041989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041989: Denominators of continued‑fraction convergents to sqrt(517). */
/* Algorithm: compute the periodic continued‑fraction coefficients of √517 once,
   then use the recurrence q_n = a_n * q_{n-1} + q_{n-2}. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static unsigned int period[64];
    static size_t period_len = 0;

    /* Build the periodic part of the continued fraction of √517 once. */
    if (!period_len)
    {
        const unsigned long long a0 = 22ULL;   /* floor(sqrt(517)) */
        unsigned long long m = 0, d = 1, a = a0;
        do
        {
            m = d * a - m;
            d = (517ULL - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (unsigned int)a;
        } while (a != 2ULL * a0);
    }

    seq_t prevprev = 0;   /* q_{-1}   */
    seq_t prev     = 1;   /* q_{0}    */

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned int coeff = period[(size_t)(i - 1) % period_len];
        seq_t curr = prev * (seq_t)coeff + prevprev;
        prevprev = prev;
        prev = curr;
    }

    return prev;         /* q_n = a(n) */
}

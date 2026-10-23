/*
 * A040449  Continued fraction for sqrt(471).
 * https://oeis.org/A040449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction of sqrt(471) */

/*  The function returns the nth coefficient of the continued fraction
 *  expansion of √471.  a(0) = ⌊√471⌋ = 21.  For n > 0 the sequence
 *  is periodic with period length 14: 1,2,2,1,3,4,14,4,3,1,2,2,1,42.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for the continued fraction of √471 */
    const uint64_t A0      = 21;   /* ⌊√471⌋          */
    const uint64_t TWO_A0  = 42;   /* 2·A0            */

    /* Periodic part (excluding the first term a(0)=A0).  The period
     * has length 14, but we compute it generically to avoid hard‑coding
     * the values.  The array is initially empty and filled on the first
     * call. */
    static seq_t *period = NULL;
    static idx_t  period_len = 0;

    /* --- initialise the period on the first call -------------------------------- */
    if (period == NULL) {
        /* There will be at most 2*A0 terms before the repetition; that is
         * safely less than 50 for D=471. */
        period = (seq_t *)malloc(50 * sizeof(seq_t));
        if (period == NULL)
            return (seq_t)0;  /* allocation failure (unlikely) */

        /* Standard continued‑fraction algorithm for √D.
         * All intermediate variables fit in 64 bits because D=471. */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_cur = A0;

        idx_t cnt = 0;
        while (1) {
            /* Next triple (m, d, a) */
            m = d * a_cur - m;
            d = (471 - m * m) / d;
            a_cur = (A0 + m) / d;

            /* Store the coefficient a_next */
            period[cnt++] = (seq_t)a_cur;

            /* The last term of the period is 2*A0.  Stop after storing it. */
            if (a_cur == TWO_A0)
                break;
        }
        period_len = cnt;
    }

    /* --- answer the query ----------------------------------------------------- */

    if (n == 0)
        return (seq_t)A0;       /* the first term is the floor of √471 */

    /* Periodic part starts at index 1.  Use modulo arithmetic to wrap around. */
    return period[(n - 1) % period_len];
}

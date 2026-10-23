/*
 * A042618  Numerators of continued fraction convergents to sqrt(838).
 * https://oeis.org/A042618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042618: Numerators of continued fraction convergents to sqrt(838).
   Recurrence: p_{-2}=0, p_{-1}=1, p_k = a_k * p_{k-1} + p_{k-2},
   with a_0 = ⌊√838⌋ = 28 and the subsequent a_k (k≥1) being the periodic
   continued‑fraction partial quotients of √838. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy initialization of continued‑fraction data for √838. */
    static bool init = false;
    static idx_t a0 = 0;          /* floor(sqrt(838)) = 28 */
    static idx_t L = 0;           /* length of the period */
    static idx_t *period = NULL;  /* array of integer partial quotients */

    if (!init) {
        init = true;

        /* Compute floor(sqrt(838)). */
        uint64_t D = 838;
        a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= D)
            ++a0;          /* a0 = 28 */

        /* Compute the periodic part of the continued fraction for √838. */
        uint64_t m = 0, d = 1, a = a0;
        size_t cap = 16;
        period = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!period)
            abort();          /* allocation failure */

        while (true) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;           /* a = floor((a0 + m)/d) */

            if (L >= cap) {             /* grow buffer if necessary */
                cap <<= 1;
                period = (idx_t *)realloc(period, cap * sizeof(idx_t));
                if (!period)
                    abort();
            }
            period[L++] = (idx_t)a;     /* store the partial quotient */

            if (a == 2 * a0)            /* period terminates */
                break;
        }
    }

    /* Compute the k-th numerator p_k by the linear recurrence. */
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p_cur;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t ak = (k == 0) ? a0 : period[(k - 1) % L];
        p_cur = (seq_t)ak * p_prev1 + p_prev2;

        if (k == n)
            return p_cur;

        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    /* Unreachable, but keep the compiler happy. */
    return 0;
}

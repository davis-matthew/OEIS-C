/*
 * A042263  Denominators of continued fraction convergents to sqrt(657).
 * https://oeis.org/A042263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(657) */

/* The continued fraction of sqrt(657) is */
/*   √657 = [25; 1,1,1,2,1,1,5,1,4,1,5,1,1,2,1,1,1,50, (repeating) ] */
/* The period after the initial 25 has length 18.  The denominators q_n of the
 * convergents satisfy q_{-2}=1, q_{-1}=0 and for n≥0:   q_n = a_n*q_{n-1}+q_{n-2}.
 * This routine computes q_n (the OEIS a(n)) for any n≥0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Partial quotients after the initial 25, repeated every 18 terms. */
    static const unsigned short cf_period[18] = {
        1, 1, 1, 2, 1, 1, 5, 1, 4, 1, 5, 1, 1, 2, 1, 1, 1, 50
    };
    const idx_t period = 18;

    seq_t prev2 = 1;    /* q_{-2} */
    seq_t prev1 = 0;    /* q_{-1} */
    seq_t q;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        unsigned short ai;
        if (i == 0)          /* a_0 = floor(sqrt(657)) */
            ai = 25;
        else
            ai = cf_period[(i - 1) % period];

        q = (seq_t)ai * prev1 + prev2;
        prev2 = prev1;
        prev1 = q;
    }
    return prev1;       /* q_n = a(n) */
}

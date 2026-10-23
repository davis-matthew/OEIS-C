/*
 * A028085  Expansion of 1/((1-3x)(1-6x)(1-9x)(1-12x)).
 * https://oeis.org/A028085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 315*a(n-2) + 1350*a(n-3) - 1944*a(n-4),
 *            a(0)=1, a(1)=30, a(2)=585, a(3)=9450
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[4] = {
        (seq_t)1, (seq_t)30, (seq_t)585, (seq_t)9450
    };

    if (n < (idx_t)4)
        return init[(size_t)n];

    seq_t a0 = init[0];   /* a(n-4) */
    seq_t a1 = init[1];   /* a(n-3) */
    seq_t a2 = init[2];   /* a(n-2) */
    seq_t a3 = init[3];   /* a(n-1) */
    seq_t ai;             /* a(n) */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        ai = (seq_t)(30 * a3);
        ai -= (seq_t)(315 * a2);
        ai += (seq_t)(1350 * a1);
        ai -= (seq_t)(1944 * a0);

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}

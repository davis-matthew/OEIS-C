/*
 * A041580  Numerators of continued fraction convergents to sqrt(308).
 * https://oeis.org/A041580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(308). Continued
 *            fraction: √308 = [17;(1,1,4,1,1,34) repeating]. Let p_{-2}=0,
 *            p_{-1}=1, p_0=17. Then for n ≥ 1 p_n = a_n · p_{n-1} + p_{n-2},
 *            where a_n = 1,1,4,1,1,34,1,1,4,1,1,34,… (period 6). The function
 *            returns p_n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction after the first term 17. */
    static const seq_t period[6] = {(seq_t)1, (seq_t)1, (seq_t)4,
                                    (seq_t)1, (seq_t)1, (seq_t)34};

    /* Base convergents. */
    seq_t p_m2 = (seq_t)0;   /* p_{-2} */
    seq_t p_m1 = (seq_t)1;   /* p_{-1} */
    const seq_t a0 = (seq_t)17;
    seq_t p0 = a0 * p_m1 + p_m2;   /* p_0 = 17 */

    /* Return promptly for the first term. */
    if (n == 0)
        return p0;

    /* For n ≥ 1. */
    seq_t prev2 = p_m1;   /* initially p_{-1} */
    seq_t prev1 = p0;     /* initially p_0 */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t idx = (k - 1) % (idx_t)6;
        seq_t ak = period[(size_t)idx];          /* a_k */
        seq_t pk = ak * prev1 + prev2;           /* p_k */

        prev2 = prev1;
        prev1 = pk;
    }

    return prev1; /* numerator of the n-th convergent */
}

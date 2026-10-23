/*
 * A041368  Numerators of continued fraction convergents to sqrt(199).
 * https://oeis.org/A041368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(199)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* a(n) returns the n-th term (n ≥ 0) of OEIS A041368.  */
seq_t a(idx_t n)
{
    /* Partial quotients after a0 = 14 are periodic with period 20:   */
    /* 9, 2, 1, 2, 2, 5, 4, 1, 1, 13, 1, 1, 4, 5, 2, 2, 1, 2, 9, 28 */
    static const seq_t period[20] = {
        (seq_t)9, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)5, (seq_t)4, (seq_t)1, (seq_t)1, (seq_t)13,
        (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)5, (seq_t)2,
        (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)28
    };

    seq_t prev2 = (seq_t)0;          /* p_{k-2} */
    seq_t prev1 = (seq_t)1;          /* p_{k-1} */
    seq_t pk;                        /* current numerator */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;                    /* the k-th partial quotient */
        if (k == 0) {
            ak = (seq_t)14;          /* a0 */
        } else {
            /* period is 20; use modulo to get the k-th partial quotient */
            size_t pos = (size_t)(k - 1) % 20;
            ak = period[pos];
        }
        pk = ak * prev1 + prev2;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = pk;
    }

    return prev1;   /* prev1 holds p_n after the loop */
}

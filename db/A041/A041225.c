/*
 * A041225  Denominators of continued fraction convergents to sqrt(124).
 * https://oeis.org/A041225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=7. The continued fraction of sqrt(124) is [11;
 *            7,2,1,1,1,3,1,4,1,3,1,1,1,2,7,22] repeating. The partial
 *            quotients q_k are q_0=11 and, for k≥1, q_k = period[(k‑1) mod
 *            16], where period = {7,2,1,1,1,3,1,4,1,3,1,1,1,2,7,22}. The
 *            denominators a(n) of the convergents satisfy a(n) = q_{n‑1...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* partial quotients of sqrt(124) after the initial 11,
       repeated with period 16; 22 appears as the last element
       of the period (2 * 11). */
    static const seq_t period[16] = {
        7, 2, 1, 1, 1, 3, 1, 4,
        1, 3, 1, 1, 1, 2, 7, 22
    };

    seq_t prev2 = 1;   /* a(-2) in the recurrence */
    seq_t prev1 = 0;   /* a(-1) in the recurrence */
    seq_t cur;         /* a(k) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t q;       /* partial quotient q_k */
        if (k == 0) {
            q = 11;    /* a_0 of the continued fraction */
        } else {
            /* period is a power of two, use bitwise AND for modulus */
            q = period[(k - 1) & 15];
        }
        cur = q * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

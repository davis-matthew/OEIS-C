/*
 * A042823  Denominators of continued fraction convergents to sqrt(942).
 * https://oeis.org/A042823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: denominators of continued‑fraction convergents to sqrt(942)
   The continued fraction of sqrt(942) is [30; (1,2,4,20,4,2,1,60) repeating].
   Let q_{-2}=1, q_{-1}=0.
   For k ≥ 0:
       a_k = 30           if k==0
       a_k = period[(k-1) mod 8]   otherwise
       q_k = a_k*q_{k-1} + q_{k-2}
   Then a(n) = q_n.                                            */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* period of the continued fraction after the integer part 30 */
    static const seq_t period[] = {1, 2, 4, 20, 4, 2, 1, 60};
    const size_t L = sizeof period / sizeof period[0];

    seq_t qnm2 = 1;   /* q_{k-2}  */
    seq_t qnm1 = 0;   /* q_{k-1}  */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)30;
        } else {
            ak = period[(size_t)(k - 1) % L];
        }
        seq_t qk = ak * qnm1 + qnm2;
        qnm2 = qnm1;
        qnm1 = qk;
    }
    return qnm1;
}

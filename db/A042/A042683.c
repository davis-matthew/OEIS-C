/*
 * A042683  Denominators of continued fraction convergents to sqrt(871).
 * https://oeis.org/A042683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(871).
 *            Computation is performed by the standard continued‑fraction
 *            recurrence for quadratic irrationals
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Variables used in the continued fraction algorithm for sqrt(871). */
    const uint64_t D = 871;          /* The radicand. */
    const uint64_t sqrtD = 29;       /* floor(sqrt(871)). */
    uint64_t m = 0;                  /* Current m in the algorithm. */
    uint64_t d = 1;                  /* Current d in the algorithm. */
    uint64_t a_part = sqrtD;         /* Current partial quotient a_k. */

    /* Recurrence for convergent denominators:
       q_{-2} = 1, q_{-1} = 0, q_k = a_k * q_{k-1} + q_{k-2}   */
    seq_t q_prev2 = 1;   /* q_{-2} */
    seq_t q_prev1 = 0;   /* q_{-1} */
    seq_t q_curr;

    for (idx_t i = 0; i <= n; ++i) {
        /* Compute the denominator for the i-th convergent. */
        q_curr = (seq_t)a_part * q_prev1 + q_prev2;

        /* If the requested index is reached, return the value. */
        if (i == n)
            return q_curr;

        /* Prepare the next partial quotient using the standard
           recurrence for quadratic irrationals. */
        m = d * a_part - m;                     /* m_{k+1} = d_k * a_k - m_k */
        d = (D - m * m) / d;                    /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        a_part = (sqrtD + m) / d;               /* a_{k+1} = floor((sqrt(D)+m_{k+1})/d_{k+1}) */

        /* Shift the previous denominators for the next iteration. */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    /* The loop always returns before reaching this point. */
    return 0;
}

/*
 * A042903  Denominators of continued fraction convergents to sqrt(983).
 * https://oeis.org/A042903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the (n‑th) convergent of the regular
 *            continued fraction of √983. Equivalent linear recurrence (valid
 *            for all n>15): a(n) = 568176*a(n‑8) – a(n‑16) The algorithm
 *            below computes the terms via the standard CF recurrence without
 *            using the linear recurrence directly, thus achievi...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(983) */
    const int64_t D   = 983;  /* square‑free part */
    const int64_t a0  = 31;   /* floor(sqrt(D)) */

    /* continued fraction state variables (fit in 64‑bit signed) */
    int64_t m = 0;          /* m_{k-1} */
    int64_t d = 1;          /* d_{k-1} */
    int64_t a = a0;         /* a_{k-1} (initially a0) */

    /* convergent denominators: q_{-1}=0, q_0=1 */
    seq_t q_prev2 = 0;      /* q_{k-1} */
    seq_t q_prev1 = 1;      /* q_k   */

    if (n == 0)
        return q_prev1;    /* q_0 = 1 */

    /* iterate k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute next CF triple */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* new denominator: q_{k+1} = a_k * q_k + q_{k-1} */
        seq_t q_curr = (seq_t)a * q_prev1 + q_prev2;

        /* shift for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    return q_prev1;
}

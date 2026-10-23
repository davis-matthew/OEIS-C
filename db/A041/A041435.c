/*
 * A041435  Denominators of continued fraction convergents to sqrt(233).
 * https://oeis.org/A041435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent to sqrt(233)
 *            Recurrence: a(n) = 46312*a(n-11) + a(n-22) (n > 21) Initial
 *            values for 0 ≤ n ≤ 21 are generated from the continued fraction
 *            of sqrt(233). The function is valid for any n ≥ 0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* buffer that keeps the last 22 computed terms */
    seq_t buf[22];

    /* constants for sqrt(233) */
    const seq_t D  = 233;          /* D = 233 */
    const seq_t a0 = 15;           /* floor(sqrt(233)) = 15 */

    /* initialise the first two terms */
    buf[0]   = 1;                  /* a(0) */
    seq_t prev2 = 0;               /* q_{-1} */
    seq_t prev1 = 1;               /* q_{ 0} */

    /* variables for the continued‑fraction algorithm */
    seq_t m = 0;
    seq_t d = 1;
    seq_t ak = a0;                 /* current a_k in the CF */

    /* compute the terms up to n or up to the 22nd term (index 21) */
    idx_t limit = (n < 21) ? n : 21;
    for (idx_t k = 1; k <= limit; ++k)
    {
        /* update m, d, and a_k (the k-th partial quotient) */
        m = d * ak - m;
        d = (D - m * m) / d;
        ak = (a0 + m) / d;

        /* denominator of the k-th convergent */
        seq_t qk = ak * prev1 + prev2;
        buf[k % 22] = qk;

        /* shift for the next iteration */
        prev2 = prev1;
        prev1 = qk;
    }

    /* if the requested n is within the first 22 terms, return it */
    if (n <= 21)
        return buf[n % 22];

    /* otherwise use the linear recurrence for the remaining terms */
    for (idx_t k = 22; k <= n; ++k)
    {
        /* buf[k % 22] currently holds a(k-22); buf[(k-11)%22] holds a(k-11) */
        seq_t val = 46312 * buf[(k - 11) % 22] + buf[k % 22];
        buf[k % 22] = val;
    }

    /* buf[n % 22] now contains a(n) */
    return buf[n % 22];
}

/*
 * A042423  Denominators of continued fraction convergents to sqrt(739).
 * https://oeis.org/A042423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominator of convergents to sqrt(739) via continued fraction
 *            recurrence
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The continued‑fraction coefficients of sqrt(739) are generated
       by the standard "m,d,a" recurrence.  The sequence of denominators
       q_k satisfies:
           q_{-1} = 0,  q_0 = 1
           q_k    = a_k * q_{k-1} + q_{k-2}   (k >= 1)
       where a_k are the partial quotients of sqrt(739).  */
    if (n == 0)   /* a(0) = 1 */
        return (seq_t)1;

    /* constants for sqrt(739) */
    const uint64_t D    = 739;
    uint64_t m = 0;                     /* current m */
    uint64_t d = 1;                     /* current d */
    uint64_t a0, a;                     /* a0 = floor(sqrt(D)), a = current partial quotient */

    /* integer sqrt of D */
    a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        a0++;
    a = a0;

    seq_t prev2 = (seq_t)0;   /* q_{k-2} */
    seq_t prev1 = (seq_t)1;   /* q_{k-1} */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute next partial quotient a_k */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* compute q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t curr = (seq_t)a * prev1 + prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;  /* a(n) = q_n */
}

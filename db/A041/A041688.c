/*
 * A041688  Numerators of continued fraction convergents to sqrt(364).
 * https://oeis.org/A041688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction of sqrt(364)
 * Range:     u8 none, u16 n <= 7, u32 n <= 14, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 191 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Initial data for sqrt(364) */
    const idx_t S = 364;                     /* D = 364 */
    idx_t m = 0;
    idx_t d = 1;
    seq_t a0 = (seq_t)floor_sqrt((seq_t)S);  /* floor(sqrt(364)) = 19 */
    seq_t a_k = a0;                          /* partial quotient a_k */

    if (n == 0)
        return a0;

    /* Convergent recurrence: p(-2)=0, p(-1)=1, q(-2)=1, q(-1)=0 */
    seq_t p_nm2 = 0, p_nm1 = 1;
    seq_t q_nm2 = 1, q_nm1 = 0;

    seq_t pk, qk;
    idx_t k = 0;
    for (;; ++k)
    {
        /* p_k = a_k * p_{k-1} + p_{k-2} */
        pk = a_k * p_nm1 + p_nm2;
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        qk = a_k * q_nm1 + q_nm2;

        if (k == n)
            return pk;

        /* shift for next iteration */
        p_nm2 = p_nm1; p_nm1 = pk;
        q_nm2 = q_nm1; q_nm1 = qk;

        /* compute next partial quotient a_{k+1} */
        m = d * (idx_t)a_k - m;               /* m = d*a_k - m */
        d = (S - m * m) / d;                  /* d = (S - m^2) / d */
        a_k = (a0 + m) / d;                   /* a_k = (a0 + m) / d */
    }
}

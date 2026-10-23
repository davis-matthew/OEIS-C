/*
 * A049612  a(n) = T(n,3), array T as in A049600.
 * https://oeis.org/A049612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * ((n-1)^3/48 + 5*(n-1)^2/16 + 7*(n-1)/6 + 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence from OEIS: a(n+1) = 2^n*n^3/48 + 5*2^n*n^2/16 + 7*2^n*n/6 + 2^n, for n>=1 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t m = n - 1;                 /* m = n-1 >= 0 for n>=1 */
    seq_t p2 = pow2(m);              /* 2^m */

    /* Convert m to seq_t for operations */
    seq_t ms   = (seq_t)m;
    seq_t m_sq = ms * ms;
    seq_t m_cu = m_sq * ms;

    /* Compute the numerator of the inner fraction:
       m^3 + 15*m^2 + 56*m + 48, then divide by 48 later. */
    seq_t sum = m_cu + (seq_t)15 * m_sq + (seq_t)56 * ms + (seq_t)48;

    /* Multiply by 2^m and divide by 48 (division exact). */
    seq_t result = p2 * sum;
    result /= (seq_t)48;

    return result;
}

/*
 * A042272  Numerators of continued fraction convergents to sqrt(662).
 * https://oeis.org/A042272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = a_{n-1} * q_n + a_{n-2} where {q_n} are the partial
 *            quotients of the continued fraction of sqrt(662). Initial terms:
 *            a(0) = 25, a(1) = 26, ... Computation uses the standard
 *            recurrence with no lookup table
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit values.  Works for all unsigned 64‑bit inputs. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Numerators of continued fraction convergents to sqrt(662). */
seq_t a(idx_t n)
{
    /* 662 is the constant D for which we take the square root. */
    const uint64_t D   = 662;

    /* a0 = floor(sqrt(D)) . */
    uint64_t a0        = isqrt_u64(D);

    /* Classical continued‑fraction variables. */
    uint64_t m          = 0;        /* m_k */
    uint64_t d          = 1;        /* d_k */
    uint64_t q          = a0;       /* q_k, the current partial quotient */

    /* Recurrence variables: p_{-2} = 0, p_{-1} = 1. */
    seq_t p_prev2 = (seq_t)0;       /* p_{i-2}  */
    seq_t p_prev1 = (seq_t)1;       /* p_{i-1}  */
    seq_t cur     = (seq_t)a0 * p_prev1 + p_prev2;   /* p_0   */

    if (n == 0)
        return cur;                /* a(0) */

    /* Iterate from i = 1 up to n.  Each loop computes p_i. */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Next partial quotient.  */
        m = q * d - m;
        d = (D - m * m) / d;
        q = (a0 + m) / d;

        /* Recurrence: p_i = q * p_{i-1} + p_{i-2}.  */
        seq_t next = (seq_t)q * cur + p_prev1;

        /* Advance indices for next iteration.  */
        p_prev1 = cur;
        cur     = next;
    }

    return cur;
}

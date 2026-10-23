/*
 * A175386  a(n) = denominator of Sum_{i=1..n} (1/i)*C(2n-i-1,i-1).
 * https://oeis.org/A175386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 2*n / gcd(L(2n)-1, 2*n) where L(k) is the Lucas sequence
 * Range:     u8 n <= 64, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominator of the sum Σ_{i=1..n} (1/i) * C(2n-i-1, i-1). */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* m = 2*n (denominator base).  Use seq_t for arithmetic but n is small. */
    idx_t m_idx = 2 * n;
    seq_t m = (seq_t)m_idx;                 /* 2*n as seq_t */

    /* Compute Lucas(2n) modulo m.  Recurrence: L(0)=2, L(1)=1. */
    seq_t a0 = 2 % m;
    seq_t a1 = 1 % m;
    seq_t a_prev = a0;
    seq_t a_curr = a1;
    for (idx_t i = 2; i <= m_idx; ++i)
    {
        seq_t tmp = a_prev + a_curr;
        if (tmp >= m) tmp -= m;            /* modulo addition */
        a_prev = a_curr;
        a_curr = tmp;
    }

    /* a_curr now holds Lucas(2n) mod m.  Compute difference with 1. */
    seq_t diff;
    if (a_curr == 0)
    {
        /* Lucas(2n) mod m == 0, so (0 - 1) mod m == m-1. */
        diff = m - 1;
    }
    else
    {
        diff = a_curr - 1;                 /* a_curr > 0 */
    }

    /* gcd of diff and m. */
    seq_t g = gcd(diff, m);

    /* a(n) = m / g.  Division is exact. */
    return m / g;
}

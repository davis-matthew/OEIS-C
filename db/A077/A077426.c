/*
 * A077426  Nonsquare integers k such that the continued fraction
 *          (sqrt(k)+1)/2 has odd period length.
 * https://oeis.org/A077426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Scan successive odd integers k, skipping perfect squares.
 * For each k, compute the period length of the continued fraction of sqrt(k)
 * by the classical recurrence.  The sequence a(n) is the nth odd,
 * nonsquare integer whose sqrt continued fraction has odd period length. */
 
#include <stdint.h>
#include <stdbool.h>
 
/* Integer square root (floor) for 64‑bit unsigned integers. */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}
 
/* Test whether the continued fraction of sqrt(D) has odd period length.
 * Returns false for perfect squares.  The test is performed using the
 * standard recurrence for continued fractions of sqrt(D). */
static inline bool has_odd_sqrt_period(idx_t D)
{
    idx_t a0 = int_sqrt(D);
    if (a0 * a0 == D)            /* perfect square */
        return false;
 
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    bool odd = false;
    while (1)
    {
        /* New m = d*a - m */
        m = (unsigned __int128)d * a - m;
        /* New d = (D - m*m) / d */
        d = (D - (idx_t)((unsigned __int128)m * m)) / d;
        /* New a = floor((a0 + m) / d) */
        a = (a0 + m) / d;
        odd = !odd;            /* toggle parity of period length */
        if (a == 2 * a0)       /* period ends when a == 2*a0 */
            break;
    }
    return odd;
}
 
/* Return the n-th term of A077426 (offset 1).  The function performs an
 * on‑demand search; it need not be called in increasing order. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n)
    {
        ++k;
        if ((k & 1) == 0)               /* skip even integers */
            continue;
        if (!has_odd_sqrt_period(k))
            continue;
        ++count;
    }
    return (seq_t)k;
}

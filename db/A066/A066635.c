/*
 * A066635  Distance from n to closest square different from n.
 * https://oeis.org/A066635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = 2*sqrt(n)-1 if n is a perfect square; otherwise a(n) =
 *            min(n - ⌊√n⌋², (⌊√n⌋+1)² - n) (offset 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns ⌊√x⌋.  Uses binary search and 128‑bit arithmetic to avoid
   overflow. */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x < (((idx_t)1) << 32) ? x : (((idx_t)1) << 32) - 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    idx_t m   = int_sqrt(n);          /* floor(sqrt(n)) */
    idx_t m_sq = m * m;               /* m², fits in 64 bits */

    if (m_sq == n) {                  /* n is a perfect square */
        return (seq_t)(2 * m - 1);
    } else {
        idx_t lower = n - m_sq;                           /* distance to lower square */
        idx_t upper = (m + 1) * (m + 1) - n;              /* distance to upper square */
        return (seq_t)(lower <= upper ? lower : upper);
    }
}

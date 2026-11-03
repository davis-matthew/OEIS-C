/*
 * A189366  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(2), t=(1+sqrt(5))/2.
 * https://oeis.org/A189366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/r/t⌋ + ⌊n*s/t⌋ where r = 1, s = √2, t = (1+√5)/2
 *            The formula simplifies to: a(n) = n + ⌊n/φ⌋ + ⌊n*√2/φ⌋, with φ =
 *            (1+√5)/2. The following implementation evaluates the two floor
 *            terms exactly using integer arithmetic and the available
 *            floor_sqrt utility
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 584, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9503 (2026-11-01)
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

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Guard against the undefined offset (n < 1) */
    if (n < 1) return 0;

    /* Convert to seq_t early to avoid multiple casts */
    const seq_t sn = (seq_t)n;
    const seq_t nsq = sn * sn;            /* n² */

    /* ----------- floor(n/φ) via sqrt(5) ---------------- */
    /*  floor(n/φ) = floor(n*(φ-1)) = floor((n*√5 - n)/2) */
    const seq_t sqrt5_nsq  = floor_sqrt(nsq * (seq_t)5); /* ⌊n√5⌋   */
    const seq_t term1 = (sqrt5_nsq - sn) / (seq_t)2;     /* ⌊n/φ⌋   */

    /* ----------- floor(n*√2/φ) via sqrt(10) and sqrt(2) ----- */
    /*  n*√2/φ = n*(√10 - √2)/2  */
    const seq_t s10 = floor_sqrt(nsq * (seq_t)10); /* ⌊n√10⌋   */
    const seq_t s2  = floor_sqrt(nsq * (seq_t)2);  /* ⌊n√2⌋    */
    const seq_t m   = s10 - s2;                    /* integer diff */

    seq_t term2;
    if (m & 1) {   /* m odd → ⌊diff/2⌋ = ⌊m/2⌋ */
        term2 = m / (seq_t)2;
    } else {       /* m even → need to decide if we must subtract 1 */
        /* Compute L = 8*n² - m²  (positive for all valid n) */
        const seq_t L = (seq_t)8 * nsq - m * m;
        /* Compare L² with 8*m²*n² to detect negative δ */
        const seq_t L2  = L * L;
        const seq_t RHS = (seq_t)8 * m * m * nsq;

        if (L2 < RHS) {
            term2 = (m / (seq_t)2) - (seq_t)1;   /* δ < 0 → subtract 1 */
        } else {
            term2 = m / (seq_t)2;                /* δ ≥ 0   */
        }
    }

    return sn + term1 + term2;
}

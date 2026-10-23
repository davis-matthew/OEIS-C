/*
 * A121294  a(m^2) = m^3; a(m^2+k) = m^3 + k*m, 0 <= k <= m; a(m*(m+1)) =
 *          (m+1)*m^2; a(m*(m+1)+k) = (m+1)m^2 + k*(2m+1), 0 <= k <= m+1;
 *          a((m+1)^2) = (m+1)^3.
 * https://oeis.org/A121294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1 * a(m²) = m³ * a(m² + k) = m³ + k·m, 0 ≤ k ≤ m *
 *            a(m(m+1)) = (m+1)·m² * a(m(m+1)+k)= (m+1)·m² + k·(2m+1), 0 ≤ k ≤
 *            m+1 * a((m+1)²) = (m+1)³ * (n is 1‑based index)
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor square root for 64‑bit unsigned */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    if (x == (idx_t)0)
        return (idx_t)0;
    idx_t lo = (idx_t)0;
    idx_t hi = (x >> 1) + (idx_t)1;  /* hi is always ≥ √x */
    idx_t ans = (idx_t)0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == (idx_t)0) {          /* avoid division by zero for x < 2 */
            ans = (idx_t)0;
            break;
        }
        if (mid <= x / mid) {           /* mid*mid ≤ x */
            ans = mid;
            lo = mid + (idx_t)1;
        } else {
            if (mid == (idx_t)0) break; /* should not happen */
            hi = mid - (idx_t)1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification */
    idx_t m = floor_sqrt_u64(n);             /* floor(sqrt(n)) */
    idx_t m_sq_idx = m * m;                   /* m² as idx_t */

    seq_t dm      = (seq_t)m;                /* m as seq_t          */
    seq_t m_sq    = dm * dm;                 /* m² as seq_t         */
    seq_t m_cube  = dm * m_sq;               /* m³ as seq_t         */
    idx_t m_sq_m  = m_sq_idx + m;            /* m² + m as idx_t     */

    if (n <= m_sq_m) {                       /* first interval    */
        seq_t k = (seq_t)(n - m_sq_idx);     /* 0 ≤ k ≤ m          */
        return m_cube + k * dm;
    } else {                                 /* second interval   */
        seq_t dm1   = dm + (seq_t)1;            /* m+1 as seq_t       */
        seq_t base  = dm1 * m_sq;              /* (m+1)·m²           */
        seq_t k     = (seq_t)(n - m_sq_m);     /* 0 ≤ k ≤ m+1        */
        seq_t mul   = (seq_t)(2 * (seq_t)m + 1);/* 2m+1                */
        return base + k * mul;
    }
}

/*
 * A187944  [nr+kr]-[nr]-[kr], where r=(1+sqrt(5))/2, k=3, [ ]=floor.
 * https://oeis.org/A187944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(n+3)φ⌋ – ⌊nφ⌋ – ⌊3φ⌋, φ = (1+√5)/2 Since ⌊3φ⌋ = 4, this
 *            reduces to a(n) = ⌊(n+3)φ⌋ – ⌊nφ⌋ – 4
 * Range:     u8 none, u16 n <= 54, u32 n <= 124, u64 n <= 124, u128 n <= 124,
 *            big any n
 * Validated: partial, pass, 124 terms through n = 124 (2026-11-01)
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

static inline seq_t floor_kphi(idx_t m)
{
    /* Compute floor(m * φ) exactly using integer arithmetic:
       φ = (1 + √5)/2, so
       floor(mφ) = floor( (m + floor(m√5)) / 2 )  */
    seq_t m_seq = (seq_t)m;
    seq_t prod  = (seq_t)5 * (m_seq * m_seq);   /* 5 * m^2  */
    seq_t sqrt_val = floor_sqrt(prod);           /* ⌊m√5⌋   */
    seq_t num = m_seq + sqrt_val;                /* m + ⌊m√5⌋ */
    return num / 2;                              /* ⌊mφ⌋    */
}

seq_t a(idx_t n)
{
    idx_t n_plus3 = n + 3;
    seq_t floor_n    = floor_kphi(n);
    seq_t floor_n3   = floor_kphi(n_plus3);
    return floor_n3 - floor_n - ((seq_t)4);
}

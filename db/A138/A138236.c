/*
 * A138236  Self-inverse integer permutation induced by Beatty sequences for
 *          Sqrt(6) and (6+Sqrt(6))/5.
 * https://oeis.org/A138236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) pairs the Beatty sequences for α = sqrt(6) and β = (6+sqrt(6))/5.
 * Let s_k = floor(k*sqrt(6))  (sequence A022840),
 *     t_k = floor((6*k + s_k)/5)  (sequence A138235).
 * For each k ≥ 1, a(s_k) = t_k and a(t_k) = s_k.
 * Thus for a given n:
 *   if n = s_k  → a(n) = t_k,
 *   else        → n = t_k  and a(n) = s_k.
 */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined, but keeps compiler happy */

    idx_t lo = (idx_t)1, hi = n;
    /* Search in the α‑Beatty sequence (s_k) */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t mid_sq = (seq_t)mid * (seq_t)mid;
        seq_t s = floor_sqrt((seq_t)6 * mid_sq);
        if (s == (seq_t)n) {
            /* partner is t_k */
            return ((seq_t)6 * (seq_t)mid + s) / (seq_t)5;
        }
        if (s < (seq_t)n)
            lo = mid + (idx_t)1;
        else
            hi = mid - (idx_t)1;
    }

    /* must be in the β‑Beatty sequence (t_k) */
    lo = (idx_t)1; hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t mid_sq = (seq_t)mid * (seq_t)mid;
        seq_t s = floor_sqrt((seq_t)6 * mid_sq);
        seq_t t = ((seq_t)6 * (seq_t)mid + s) / (seq_t)5;
        if (t == (seq_t)n) {
            /* partner is s_k */
            return s;
        }
        if (t < (seq_t)n)
            lo = mid + (idx_t)1;
        else
            hi = mid - (idx_t)1;
    }

    /* should never happen for n ≥ 1 */
    return (seq_t)n;
}

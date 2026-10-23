/*
 * A184977  a(n) = Sum_{k=1..n} floor(k*gamma) where gamma is Euler's constant
 *          (A001620).
 * https://oeis.org/A184977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊k·γ⌋ (γ = Euler's constant)
 * Range:     u8 none, u16 n <= 3, u32 n <= 2, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We approximate γ to 15 decimal places:
   γ ≈ 577215664901532 / 10^15
   The truncation error is < 10⁻¹⁵, so for n ≤ 10⁴ the error in k·γ
   is < 10⁻¹¹ and never changes the integer part.  This works for
   all seq_t types that can hold a 64‑bit integer. */

#include <stdint.h>
#include "../utils/seq_from_str.h"   /* optional: not used here */

#define GAM_NUM (seq_t)577215664901532ULL
#define GAM_DEN (seq_t)1000000000000000ULL   /* 10^15 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t prod = (seq_t)k * GAM_NUM;   /* k·γ_num   (no overflow)   */
        seq_t term = prod / GAM_DEN;       /* ⌊k·γ⌋    using integer division */
        sum += term;
    }
    return sum;
}

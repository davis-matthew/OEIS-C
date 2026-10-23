/*
 * A117137  Same as triangle in A117136, but omit final 1 from each row.
 * https://oeis.org/A117137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 1 + Σ_{i=1}^{p} 𝜔(i)·(r + i − 1) where r = ⌊√n⌋, p = n −
 *            r² (0 ≤ p ≤ 2r), and 𝜔(i) = +1 for i ≤ 2 or i even, otherwise −1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* integer floor square root using long double */
    idx_t r = (idx_t)sqrtl((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);
    idx_t r2 = r * r;
    idx_t p = n - r2;          /* offset within the row, 0 <= p <= 2r */

    seq_t sr = (seq_t)r;
    seq_t sp = (seq_t)p;

    /* total sum of all increments */
    seq_t total = sp * sr + sp * (sp - 1) / 2;

    seq_t neg_sum = 0;
    if (p >= 3) {
        idx_t last_odd = (p % 2 == 1) ? p : p - 1;
        idx_t cnt = ((last_odd - 3) / 2) + 1;       /* count of odd i in [3..p] */
        seq_t scnt = (seq_t)cnt;
        seq_t sum_odd = scnt * ((seq_t)(3 + last_odd)) / 2;
        neg_sum = scnt * sr + sum_odd - scnt;      /* Σ (r + i − 1) over negative i */
    }

    seq_t val = 1 + total - 2 * neg_sum;
    return val;
}

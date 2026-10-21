/*
 * A088961  Zigzag matrices listed entry by entry.
 * https://oeis.org/A088961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   a(n) = Zigzag matrices listed entry by entry For global index
 *            n≥1, find the unique m with Σ_{t=1}^{m‑1} t² < n ≤ Σ_{t=1}^m t².
 *            Let r = n – Σ_{t=1}^{m‑1} t²; 1 ≤ r ≤ m². Write r in row‑major
 *            order: i = (r‑1)/m + 1, j = (r‑1)%m + 1. Then a(n) =
 *            binomial(2m, m + j – i) – binomial(2m, m + i + j) if m...
 * Range:     u8 n <= 14, u16 n <= 273, u32 n <= 1783, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline unsigned __int128 sum_squares(idx_t t)
{
    return (unsigned __int128)t * (t + 1) * (2 * t + 1) / 6;
}

seq_t a(idx_t n)
{
    /* Find matrix size m */
    idx_t low = 1, high = 1;
    unsigned __int128 target = (unsigned __int128)n;
    unsigned __int128 high_b = sum_squares(high);
    while (high_b < target) {
        high <<= 1;
        high_b = sum_squares(high);
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid_b = sum_squares(mid);
        if (mid_b >= target)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Offset within Z(m) */
    unsigned __int128 prev_b = sum_squares(m - 1);
    idx_t r = n - (idx_t)prev_b;      /* r is 1‑based */
    idx_t i = (r - 1) / m + 1;
    idx_t j = (r - 1) % m + 1;

    /* Core binomial computation */
    idx_t two_n = m << 1; /* 2*m */

    int64_t k1 = (int64_t)m + (int64_t)j - (int64_t)i;           /* always 1 … 2m‑1 */
    seq_t val = binomial(two_n, (idx_t)k1);

    int64_t k2 = (int64_t)m + (int64_t)i + (int64_t)j;           /* ≤ 3m, may exceed 2m */
    if (k2 <= (int64_t)two_n)
        val -= binomial(two_n, (idx_t)k2);

    int64_t k3 = 3 * (int64_t)m + 1 - (int64_t)i - (int64_t)j;   /* may exceed 2m */
    if (k3 <= (int64_t)two_n)
        val += binomial(two_n, (idx_t)k3);

    return val;
}

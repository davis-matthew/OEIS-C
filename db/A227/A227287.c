/*
 * A227287  G.f.: Sum_{n>=0} x^(n - b(n)) * (1+x)^b(n), where b(n) =
 *          A007814(n), which is the exponent of the highest power of 2
 *          dividing n.
 * https://oeis.org/A227287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥n, m−n ≤ v₂(m)} C(v₂(m), m−n) where v₂(m) is the
 *            exponent of the highest power of 2 dividing m
 * Range:     u8 n <= 2040, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t v2_idx(idx_t n)
{
    idx_t v = 0;
    if (n == 0) return 0;
    while ((n & 1ULL) == 0ULL) { ++v; n >>= 1; }
    return v;
}

static inline idx_t floor_log2_idx(idx_t v)
{
    if (v == 0) return 0;
    idx_t r = 0;
    while (v > 1) { v >>= 1; ++r; }
    return r;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t m = n;; ++m) {
        idx_t v = v2_idx(m);
        idx_t d = m - n;                /* difference m - n */

        /* Once the difference exceeds the largest possible v₂(m),
           no further m can contribute. */
        if (d > floor_log2_idx(m)) break;

        if (d > v) continue;            /* this m does not contribute */

        /* compute binomial coefficient C(v, d) */
        seq_t term = 1;
        for (idx_t i = 1; i <= d; ++i)
            term = term * (v - d + i) / i;

        sum += term;
    }
    return sum;
}

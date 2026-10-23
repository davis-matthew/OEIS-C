/*
 * A194747  Number of k such that {k*sqrt(6)} > {n*sqrt(6)}, where { } =
 *          fractional part.
 * https://oeis.org/A194747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 1 ≤ k < n and {k√6} > {n√6}}
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit number, using Newton’s method */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2U) return n;
    uint64_t x = n;
    uint64_t y = (x + 1U) >> 1U;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1U;
    }
    return x;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0, as defined */

    /* pre‑compute floor(n√6) */
    uint64_t prod_n = 6ULL * n * n;        /* safe for n up to ~1e9 */
    uint64_t floor_n = isqrt_u64(prod_n);

    /* count k with 1 ≤ k < n satisfying the inequality */
    uint64_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        uint64_t prod_k = 6ULL * k * k;
        uint64_t floor_k = isqrt_u64(prod_k);

        idx_t t = n - k;                   /* t = n-k > 0 */
        uint64_t prod_t = 6ULL * t * t;
        uint64_t floor_t = isqrt_u64(prod_t);

        /* condition: floor_n - floor_k == floor_t + 1 */
        if (floor_n - floor_k == floor_t + 1U)
            ++count;
    }

    return (seq_t)count;                  /* cast to seq_t (possible truncation) */
}

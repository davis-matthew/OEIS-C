/*
 * A205376  Ordered differences of distinct odd squares, stored in triangle.
 * https://oeis.org/A205376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*r+1)² - (2*k+1)² where r = ceil( (√(1+8n)-1)/2 ) (row
 *            index, n≥1) k = n - r*(r-1)/2 - 1 (column index, 0‑based)
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t: returns floor(√x) */
static inline uint64_t isqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = (x >> 1) + 1;   /* upper bound for sqrt */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid)                /* mid*mid > x */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* OEIS sequence A205376 */
seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset safety – not used */

    /* Find smallest r such that r*(r+1)/2 >= n */
    uint64_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    if (r * (r + 1) / 2 < n) r++;     /* correct possible floor rounding */

    /* Column index (0‑based) within this row */
    uint64_t prev = (r - 1) * r / 2;
    uint64_t k = n - prev - 1;

    /* Compute the odd squares in seq_t */
    seq_t A = (seq_t)(2 * r + 1);     /* (2r+1) */
    seq_t B = (seq_t)(2 * k + 1);     /* (2k+1) */

    return A * A - B * B;
}

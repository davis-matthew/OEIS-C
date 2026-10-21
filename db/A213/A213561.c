/*
 * A213561  Rectangular array: (row n) = b**c, where b(h) = h^2, c(h) =
 *          m*(m+1)/2, m=n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) where T(i,j) = ∑_{h=1}^{j}
 *            h²·((i+j‑h)·(i+j‑h+1)/2)
 * Range:     u8 n <= 15, u16 n <= 190, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root (floor) for 64‑bit integers */
static inline idx_t floor_sqrt_idx(idx_t x) {
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32;           /* > sqrt(2^64‑1) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) low = mid + 1;
        else high = mid - 1;
    }
    return low - 1;
}

seq_t a(idx_t n) {
    /* Map the antidiagonal index n to row (i) and column (j) */
    idx_t D = 1 + 8 * n;
    idx_t sqrtD = floor_sqrt_idx(D);
    idx_t s = (1 + sqrtD) / 2;

    /* Adjust s so that (s‑1)*s/2 >= n  */
    while ((unsigned __int128)(s - 1) * s / 2 < n) ++s;
    while ((unsigned __int128)(s - 2) * (s - 1) / 2 >= n) --s;

    unsigned __int128 s128 = s;
    unsigned __int128 prev128 = (s128 - 2) * (s128 - 1) / 2;
    idx_t prev = (idx_t)prev128;                /* always fits in idx_t */

    idx_t offset = n - prev - 1;                /* 0‑based on this antidiagonal */
    idx_t row = offset + 1;                     /* i */
    idx_t col = s - row;                        /* j */

    /* Compute the convolution formula using 128‑bit arithmetic */
    unsigned __int128 C = (unsigned __int128)col;
    unsigned __int128 M = (unsigned __int128)row + (unsigned __int128)col;

    unsigned __int128 S2 = C * (C + 1) * (2 * C + 1) / 6;          /* Σ t² */
    unsigned __int128 t  = C * (C + 1) / 2;                        /* Σ t   */
    unsigned __int128 S3 = t * t;                                 /* Σ t³ */
    unsigned __int128 S4 = C * (C + 1) * (2 * C + 1) *
                          (3 * C * C + 3 * C - 1) / 30;             /* Σ t⁴ */

    unsigned __int128 term1 = (M * M + M) * S2;
    unsigned __int128 term2 = (2 * M + 1) * S3;
    unsigned __int128 result128 = (term1 - term2 + S4) / 2;

    return (seq_t)result128;
}

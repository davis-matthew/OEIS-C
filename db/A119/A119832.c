/*
 * A119832  Bi-diagonal inverse of (3n)!/(3k)!.
 * https://oeis.org/A119832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 1 if n = i*(i+3)/2 for some i >= 0; a(n) =
 *            -(3i-2)(3i-1)(3i) if n = (i^2+3i-2)/2 for some i >= 1; a(n) = 0
 *            otherwise. In particular the triangle of the inverse of
 *            (3n)!/(3k)! has only diagonal entries 1 and sub‑diagonal entries
 *            -(3i-2)(3i-1)(3i), all other entries being 0
 * Range:     i8 n <= 7, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of a 64‑bit unsigned value (floor(sqrt(x))) */
static idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = (idx_t)1 << 32; /* hi > sqrt(max 64‑bit) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    /* 1. Check for diagonal entries a(n) = 1. */
    idx_t diag_val = 9 + 8 * n;                  /* 9+8n */
    idx_t s = isqrt_u64(diag_val);
    if (s * s == diag_val && ((s - 3) & 1) == 0) {
        /* i = (s-3)/2  is an integer; diagonal entry */
        return (seq_t)1;
    }

    /* 2. Check for sub‑diagonal entries a(n) = -B_i. */
    idx_t sub_val = 9 + 8 * (n + 1);             /* 9+8(n+1) */
    idx_t t = isqrt_u64(sub_val);
    if (t * t == sub_val && ((t - 3) & 1) == 0) {
        idx_t i = (t - 3) >> 1;                 /* i >= 1 */
        if (i == 0) return (seq_t)0;           /* safety */

        /* Compute B_i = (3i-2)(3i-1)(3i). */
        unsigned long long three_i = 3ULL * (unsigned long long)i;
        unsigned long long factor = (three_i - 2ULL);
        factor *= (three_i - 1ULL);
        factor *= three_i;

        /* Return negative value. */
        long long signed_factor = (long long)factor;
        return (seq_t)(-signed_factor);
    }

    /* 3. All other positions are zero. */
    return (seq_t)0;
}

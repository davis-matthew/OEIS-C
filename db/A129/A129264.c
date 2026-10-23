/*
 * A129264  Triangle read by rows: A000012 * A115361 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A129264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of i ≤ r such that the largest odd divisor of i
 *            equals the largest odd divisor of the column c, where (r,c)
 *            satisfies n = r*(r-1)/2 + c. This equals floor_log2(⌊r /
 *            odd_part(c)⌋) - v2(c) + 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include what is needed. */
#include <stdint.h>

/* Helper: floor log2 of a positive integer.  Uses compiler builtin if
   available, otherwise a simple loop. */
static inline idx_t log2_floor(idx_t v) {
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(v);
#else
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

seq_t a(idx_t n) {
    /* Find the row r such that T(r-1) < n ≤ T(r), where T(k)=k*(k+1)/2. */
    idx_t low = 1, high = 1;
    /* Exponential search to find an upper bound. */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search for the exact row. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index c (1‑based). */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - prev_tri;

    /* Compute v2(c) – the exponent of the largest power of two dividing c. */
    idx_t e = __builtin_ctzll(c);          /* c > 0 because c ≤ r. */
    idx_t d = c >> e;                      /* odd part of c. */

    /* m = floor(r / d).  All arithmetic fits in 64 bits because r ≤ n. */
    idx_t m = r / d;

    /* t = floor(log2(m)). */
    idx_t t = log2_floor(m);

    /* a(r,c) = t - e + 1.  This is guaranteed non‑negative. */
    return (seq_t)(t - e + 1);
}

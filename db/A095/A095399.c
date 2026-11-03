/*
 * A095399  Modified juggler modified further: a(n) = (1- (n mod 2)) *
 *          floor(n^(3/4)) + (n mod 2) * floor(n^(4/3)); original exponents
 *          {1/2,3/2} are replaced with {3/4,4/3}.
 * https://oeis.org/A095399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/4)) when n is even, floor(n^(4/3)) when n is
 *            odd
 * Range:     u8 n <= 64, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   icbrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icbrt_u128.h"

static inline unsigned __int128 pow_u128_u64_4(uint64_t x) {
    return (unsigned __int128)x * x * x * x;
}

/* largest integer x with x^4 <= limit  */
static uint64_t floor_root4_u128(unsigned __int128 limit) {
    uint64_t lo = 0, hi = 1;
    /* find an upper bound hi */
    while (pow_u128_u64_4(hi) <= limit) {
        if (hi > UINT64_MAX / 2) { hi = UINT64_MAX; break; }
        hi <<= 1;
    }
    /* binary search */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (pow_u128_u64_4(mid) <= limit)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static inline unsigned __int128 pow_u128_u64_3(uint64_t x) {
    return (unsigned __int128)x * x * x;
}

/* seq_t is an unsigned integer type (uint*_t or unsigned __int128) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* guard against 0; offset is 1 */
    unsigned __int128 n128 = n;
    if ((n & 1) == 0) {           /* even */
        unsigned __int128 limit = n128 * n128 * n128;   /* n^3 */
        return (seq_t)floor_root4_u128(limit);
    } else {                       /* odd */
        unsigned __int128 limit = n128 * n128 * n128 * n128;  /* n^4 */
        return (seq_t)icbrt_u128(limit);
    }
}

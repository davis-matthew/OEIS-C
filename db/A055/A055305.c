/*
 * A055305  Number of labeled rooted trees with n nodes and 4 leaves.
 * https://oeis.org/A055305
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-4)^2 * (n-3)^2 * (n-2) * (n-1) / 1152
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation avoids overflow by cancelling the divisor
   1152 (2^7 * 3^2) as early as possible using gcd reduction. */
#include <stddef.h>

static uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* For indices below the offset, the value is irrelevant. */
    if (n < 5) return 0;

    seq_t result = 1;
    uint64_t denom = 1152;          /* 1152 = 2^7 * 3^2 */

    /* Process factorial factors 2..n */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t f = (uint64_t)i;
        uint64_t g = gcd64(f, denom);
        denom /= g;
        f /= g;
        result *= (seq_t)f;
    }

    /* Extra factors: (n-4)^2*(n-3)^2*(n-2)*(n-1) */
    uint64_t extras[6];
    extras[0] = (uint64_t)(n - 4);
    extras[1] = (uint64_t)(n - 4);
    extras[2] = (uint64_t)(n - 3);
    extras[3] = (uint64_t)(n - 3);
    extras[4] = (uint64_t)(n - 2);
    extras[5] = (uint64_t)(n - 1);

    for (int j = 0; j < 6; ++j) {
        uint64_t f = extras[j];
        uint64_t g = gcd64(f, denom);
        denom /= g;
        f /= g;
        result *= (seq_t)f;
    }

    /* After full cancellation, denom should be 1. */
    return result;
}

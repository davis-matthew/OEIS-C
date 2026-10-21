/*
 * A207735  Expansion of f(-x^2, x^3)^2 / f(x, -x^2) in powers of x where f()
 *          is Ramanujan's two-variable theta function.
 * https://oeis.org/A207735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k∈ℤ} (-1)^[k/2]·[n = (15k²‑k)/2] – (-1)^[k/2]·[n =
 *            (15k²+11k+2)/2] (where [condition] is 1 if true, 0 otherwise).
 *            The sequence returns ±1 or 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   sign_of_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sign_of_k.h"

/* Integer square root of a 128‑bit unsigned integer, result fits in 64 bits. */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (1ULL << 36); /* 2^36 ≈ 6.8e10 > √(120·(2^64−1)) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    unsigned __int128 D = (unsigned __int128)1 + (unsigned __int128)120 * n;
    uint64_t s = isqrt_u128(D);
    if ((unsigned __int128)s * s != D)                /* not a perfect square */
        return (seq_t)0;

    /* 1. Check the two possible roots of the first quadratic (E1). */
    int64_t num = 1 + (int64_t)s;
    if (num % 30 == 0) {
        int64_t k = num / 30;
        return (seq_t)sign_of_k(k);
    }
    num = 1 - (int64_t)s;
    if (num % 30 == 0) {
        int64_t k = num / 30;
        return (seq_t)sign_of_k(k);
    }

    /* 2. Check the two possible roots of the second quadratic (E2). */
    num = -11 + (int64_t)s;
    if (num % 30 == 0) {
        int64_t k = num / 30;
        return (seq_t)(-sign_of_k(k));
    }
    num = -11 - (int64_t)s;
    if (num % 30 == 0) {
        int64_t k = num / 30;
        return (seq_t)(-sign_of_k(k));
    }

    /* Should never reach here; return 0 as a fallback. */
    return (seq_t)0;
}

/*
 * A194862  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=(1+sqrt(3))/2.
 * https://oeis.org/A194862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element at index n of the permutation of (1,2,…,r) *
 *            where the permutation is formed by ordering {k·r} (fractional
 *            part), * with r = (1+√3)/2. The sequence is the concatenation of
 *            all rows. * This implementation computes a(n) for any n ≥ 1
 *            using only integer * arithmetic
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: integer square root for 64‑bit values (floor).
 * Uses binary search on 128‑bit products to avoid overflow. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = ((uint64_t)1U << 32U) - 1U; /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        else if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Helper: integer square root for 128‑bit values (floor).
 * Works for values that fit in 128 bits (<= 2^128-1). */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0, high = ((uint64_t)1U << 64U) - 1U;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        unsigned __int128 sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        else if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Global constants, initialized lazily.
 * SCALE is a power of two that allows us to represent √3 with integer
 * precision without overflow.  We use 2^62 which leaves sufficient room
 * for intermediate 128‑bit calculations. */
static const unsigned __int128 SCALE_VAL = ((unsigned __int128)1U << 62U);
static uint64_t SQRT3_SCALED = 0;
static int constants_initialised = 0;

static void initialise_constants(void)
{
    if (constants_initialised) return;
    /* Compute floor(√3 · SCALE_VAL). */
    unsigned __int128 temp = (__uint128_t)3U * SCALE_VAL * SCALE_VAL; /* 3·SCALE² */
    SQRT3_SCALED = (uint64_t)isqrt_u128(temp);
    constants_initialised = 1;
}

/* Comparator for a_k values (the fractional parts of k·r).
 * Returns -1 if a_k1 < a_k2, 1 if a_k1 > a_k2. */
static int compare_a_k(idx_t k1, idx_t k2)
{
    if (k1 == k2) return 0;

    int64_t delta = (int64_t)k1 - (int64_t)k2;           /* k1 - k2 */

    /* Compute floor( k·√3 ) for each k.  Use integer sqrt of 3·k². */
    uint64_t s1 = isqrt_u64(3ULL * k1 * k1);
    uint64_t s2 = isqrt_u64(3ULL * k2 * k2);

    /* floor( k·r ) = floor( (k + floor(k·√3)) / 2 ). */
    int64_t floor1 = (int64_t)((k1 + s1) / 2);
    int64_t floor2 = (int64_t)((k2 + s2) / 2);

    int64_t delta_floor = floor1 - floor2;
    int64_t rhs = 2 * delta_floor - delta;   /* rhs = 2·(floor1-floor2) - (k1-k2) */

    /* Compare  δ·√3  and  rhs  by scaling √3 with integer precision. */
    __int128 lhs_scaled = (__int128)delta * SQRT3_SCALED;
    __int128 rhs_scaled = (__int128)rhs * SCALE_VAL;
    __int128 diff = lhs_scaled - rhs_scaled;   /* sign of D */

    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;   /* practically cannot happen for distinct k */
}

/* Main function: compute the OEIS sequence A194862. */
seq_t a(idx_t n)
{
    initialise_constants();

    /* Find row r such that r(r+1)/2 ≥ n.  Use a simple loop – r ≤ ~44721
     * for n up to 1e9, well within time limits. */
    idx_t r = 1;
    while ((__uint128_t)r * (r + 1) / 2 < n) r++;

    /* Position within that row. */
    idx_t prev_tri = (r * (r - 1)) / 2;
    idx_t pos = n - prev_tri;   /* 1‑based */

    /* Build array of indices 1..r. */
    idx_t *arr = (idx_t *)malloc(sizeof(idx_t) * (size_t)r);
    if (!arr) return (seq_t)0; /* fallback; should never happen */

    for (idx_t i = 0; i < r; ++i) arr[i] = i + 1;

    /* Insertion sort on the array using compare_a_k.  r is small (< 1500) */
    for (idx_t i = 1; i < r; ++i) {
        idx_t key = arr[i];
        idx_t j = i;
        while (j > 0 && compare_a_k(arr[j - 1], key) > 0) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }

    seq_t result = (seq_t)arr[pos - 1];
    free(arr);
    return result;
}

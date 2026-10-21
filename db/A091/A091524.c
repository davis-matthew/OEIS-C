/*
 * A091524  a(m) is the multiplier of sqrt(2) in the constant alpha(m) =
 *          a(m)*sqrt(2) - b(m), where alpha(m) is the value of the constant
 *          determined by the binary bits in the recurrence associated with
 *          the Graham-Pollak sequence.
 * https://oeis.org/A091524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Beatty partition of the integers. Let α = 1 + √2 and β = 1 +
 *            1/√2. Then the sequences ⌊nα⌋ and ⌊nβ⌋ partition ℕ. In
 *            particular: a(⌊nα⌋) = n a(⌊nβ⌋) = n The function below
 *            implements this definition without using floating point
 *            arithmetic, using integer square roots only
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 128‑bit unsigned values.
   Returns ⌊√n⌋. */
static inline uint64_t isqrt128(__uint128_t n)
{
    uint64_t lo = 0, hi = (uint64_t)-1;         /* 2⁶⁴–1 */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ⌊n·α⌋ where α = 1 + √2.  Without floating point: 
   ⌊nα⌋ = n + ⌊√(2n²)⌋. */
static inline uint64_t floor_n_alpha(uint64_t n)
{
    __uint128_t val = (__uint128_t)2 * n * n;     /* 2·n² */
    uint64_t sqrt_term = isqrt128(val);           /* ⌊√(2n²)⌋ */
    return n + sqrt_term;
}

/* ⌊n·β⌋ where β = 1 + 1/√2.
   Using β = 1 + √2/2, we get
   ⌊nβ⌋ = n + ⌊⌊n²/2⌋½⌋. */
static inline uint64_t floor_n_beta(uint64_t n)
{
    __uint128_t prod = (__uint128_t)n * n;
    /* ⌊n²/2⌋: shift right is integer division. */
    uint64_t sqrt_term = isqrt128(prod >> 1);
    return n + sqrt_term;
}

/* Binary search for the index n such that ⌊nα⌋ = k
   (returns 0 if no such n). */
static uint64_t index_in_alpha(uint64_t k)
{
    uint64_t lo = 1, hi = k + 1; /* hi ensures ⌊hiα⌋ > k */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (floor_n_alpha(mid) <= k)
            lo = mid + 1;
        else
            hi = mid;
    }
    uint64_t n = lo - 1;
    if (n >= 1 && floor_n_alpha(n) == k)
        return n;
    return 0;
}

/* Binary search for the index n such that ⌊nβ⌋ = k
   (guaranteed to exist when k is not in the α‑sequence). */
static uint64_t index_in_beta(uint64_t k)
{
    uint64_t lo = 1, hi = k + 1;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (floor_n_beta(mid) <= k)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --------------------------------------------------------------- 
   Public routine required by the test harness.
   --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but safe guard */

    uint64_t k = n;

    uint64_t idx = index_in_alpha(k);
    if (idx != 0) /* found in α‑sequence */
        return (seq_t)idx;

    /* otherwise it must be in β‑sequence */
    idx = index_in_beta(k);
    return (seq_t)idx;
}

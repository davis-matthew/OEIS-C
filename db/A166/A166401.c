/*
 * A166401  Positive integers n where (the largest divisor of n that is <=
 *          sqrt(n)) divides (the smallest divisor of n that is >= sqrt(n)).
 * https://oeis.org/A166401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 7095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers n where
 *   (the largest divisor of n that is ≤ sqrt(n)) divides
 *   (the smallest divisor of n that is ≥ sqrt(n)).
 * a(1) = 1, a(2) = 2, a(3) = 3, …
 *
 * We generate terms on the fly: for every candidate integer m we
 * find its largest divisor ≤ √m, let q = m / d; the candidate
 * satisfies the property iff q % d == 0.  The n‑th such integer is
 * returned.  No memoisation or external helpers are required. */

#include <stdbool.h>
#include <math.h>

/* --- Integer square root for u64 ------------------------------------- */
static idx_t isqrt_u64(idx_t v)
{
    /* Binary search for the largest s with s*s ≤ v.
     * Uses __uint128_t to avoid overflow. */
    idx_t lo = 0;
    idx_t hi = (idx_t)1u << 32;          /* √(2^64−1) < 2^32 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __uint128_t prod = (__uint128_t)mid * mid;
        if (prod <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* --- Test whether a candidate satisfies the property ---------------- */
static bool good_candidate(idx_t m)
{
    if (m == 1)           /* 1 has the sole divisor 1 which trivially
                              satisfies the condition */  return true;

    idx_t d = isqrt_u64(m);          /* largest divisor ≤ sqrt(m) */
    for (; d >= 1; --d) {
        if (m % d == 0) {
            idx_t q = m / d;         /* complementary divisor ≥ sqrt(m) */
            return (q % d == 0);
        }
    }
    return false;   /* unreachable, kept to satisfy the compiler */
}

/* --- Public sequence routine ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; this case never used */

    idx_t found = 0;              /* how many valid numbers found */
    idx_t candidate = 0;          /* current integer being tested */

    while (found < n) {
        ++candidate;
        if (good_candidate(candidate))
            ++found;
    }

    return (seq_t)candidate;
}

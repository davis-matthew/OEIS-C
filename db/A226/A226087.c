/*
 * A226087  Number of values k in base n for which the sum of digits of k =
 *          sqrt(k).
 * https://oeis.org/A226087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = # { m : m ≥ 1, m² < n³ and sum of the base‑n * digits of
 *            m² equals m }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is computed by iterating over all square numbers k = m²
 *  with k < n³.  Because k < n³ any representation of k in base n uses
 *  at most 3 digits, hence the digit sum calculation is cheap.
 *  Complexity: O(n^1.5) per call, which is < 2×10⁶ iterations for n ≤ 10000.  */

#include <stdint.h>

/* integer square‑root for a 64‑bit unsigned integer. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0;
    uint64_t hi = (x >> 1) | 1ULL;          /* hi² > x for x > 0          */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= (__uint128_t)x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 2)                /* offset is 2, behaviour below 2 is irrelevant */
        return 0;

    uint64_t n64 = (uint64_t)n;

    /* n³ may overflow 64 bits for huge n; we only need it for n ≤ ~10⁴
     * (the test limits).  Use 128‑bit arithmetic for safety.  */
    __uint128_t prod = (__uint128_t)n64 * n64 * n64;        /* n³          */
    uint64_t max_k = (uint64_t)(prod - 1);                  /* k < n³      */

    uint64_t limit = isqrt_u64(max_k);                     /* max m       */

    seq_t count = 0;                                       /* result      */

    for (uint64_t m = 1; m <= limit; ++m) {
        uint64_t k   = m * m;          /* k = m² is safe for our n range                */
        uint64_t sum = 0;
        uint64_t temp = k;
        while (temp) {                /* at most 3 iterations because k < n³          */
            sum += temp % n64;
            temp /= n64;
        }
        if (sum == m)
            count++;
    }
    return count;
}

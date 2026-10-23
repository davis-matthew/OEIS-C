/*
 * A137526  Triangle read by rows: T(n,k) = n! * [t^n*x^k] (1-t^2) * exp(t*x).
 * https://oeis.org/A137526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  less, tabl, sign, easy
 * Formula:   a(n) = 1 if n = j(j+3)/2 (j ≥ 0), a(n) = – j(j+1) if n =
 *            j(j+5)/2 (j ≥ 1), otherwise a(n) = 0
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_u64(uint64_t x)
{
    if (x == 0ULL) return 0ULL;
    uint64_t low = 0, high = 1;
    /* Grow high until high*high > x */
    while ((unsigned __int128)high * high <= x) high <<= 1;
    /* Binary search in [low, high) */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Check for the “+1” terms: n = j(j+3)/2 */
    uint64_t d1 = 9ULL + 8ULL * (uint64_t)n;
    uint64_t s1 = isqrt_u64(d1);
    if (s1 * s1 == d1 && ((s1 - 3ULL) & 1ULL) == 0ULL) {
        /* j = (s1 – 3)/2 is always non‑negative here */
        return (seq_t)1;
    }

    /* Check for the “–” terms: n = j(j+5)/2 */
    uint64_t d2 = 25ULL + 8ULL * (uint64_t)n;
    uint64_t s2 = isqrt_u64(d2);
    if (s2 * s2 == d2 && ((s2 - 5ULL) & 1ULL) == 0ULL) {
        uint64_t j = (s2 - 5ULL) / 2ULL;
        uint64_t val = j * (j + 1ULL);
        return -( (seq_t)val );
    }

    return (seq_t)0;
}

/*
 * A018796  Smallest square that begins with n.
 * https://oeis.org/A018796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 9087, u64 n <= 9087,
 *            u128 n <= 9087, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest square whose decimal representation
   begins with the decimal digits of the argument n.
   (OEIS A018796) */

#include <stdint.h>

/* Count decimal digits of a 64‑bit integer (n > 0). */
static unsigned int digits_u64(uint64_t x)
{
    unsigned int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* Integer floor of sqrt(x) for 0 <= x < 2^64. */
static uint64_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967296ULL;   /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Integer ceil of sqrt(x) for 0 <= x < 2^64. */
static uint64_t ceil_sqrt_uint64(uint64_t x)
{
    uint64_t r = floor_sqrt_uint64(x);
    if (r * r < x)
        r++;
    return r;
}

/* Precomputed powers of ten up to 10^18 (fits in 64 bits). */
static const uint64_t POW10[19] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Special case: the empty prefix gives the square 0. */
    if (n == 0)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;

    /* Search increasing numbers of trailing digits k. */
    for (unsigned int k = 0; ; k++) {
        /* Protect against 64‑bit overflow when computing the
           bounds of the desired square interval. */
        if (N + 1 > UINT64_MAX / POW10[k])
            break;          /* should not happen for n in the test range */

        uint64_t low  = N * POW10[k];
        uint64_t high = (N + 1) * POW10[k] - 1;

        uint64_t sqrt_low  = ceil_sqrt_uint64(low);
        uint64_t sqrt_high = floor_sqrt_uint64(high);

        if (sqrt_low <= sqrt_high) {
            /* The smallest square in the interval is sqrt_low^2. */
            unsigned __int128 sq = (unsigned __int128)sqrt_low * sqrt_low;
            return (seq_t)sq;
        }
    }

    /* Fallback – should never be reached for valid inputs. */
    unsigned __int128 sq = (unsigned __int128)N * N;
    return (seq_t)sq;
}

/*
 * A135662  a(n) = ceiling(sqrt(n) - n^(1/4)).
 * https://oeis.org/A135662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(n) - n^(1/4))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned __int128, floor(sqrt(x)) */
static uint64_t int_sqrt128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1 << 63);   /* > sqrt(max 2^127-1) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* floor( n^(1/4) * SCALE ), SCALE = 2^12, result fits in uint64_t */
static uint64_t fourth_root_scaled(idx_t n)
{
    const uint64_t SCALE      = 1ULL << 12;     /* 4096 */
    const uint64_t SCALE4     = 1ULL << 48;     /* SCALE^4 */
    unsigned __int128 N = (unsigned __int128)n * SCALE4; /* n * SCALE^4 */

    /* upper bound for the fourth root: sqrt(sqrt(N)) */
    uint64_t high = int_sqrt128(int_sqrt128(N));
    uint64_t low  = 0;

    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        unsigned __int128 mid4 = mid2 * mid2;
        if (mid4 <= N)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* behaviour below the OEIS offset is irrelevant */
        return 0;

    const uint64_t SCALE = 1ULL << 12;   /* scaling factor 2^12 */
    const uint64_t SHIFT = 12;           /* log2(SCALE) */

    /* sqrt_scaled = floor( sqrt(n) * SCALE ) = floor( sqrt( n * SCALE^2 ) ) */
    unsigned __int128 sqrt_arg = (unsigned __int128)n * ((SCALE * SCALE));   /* SCALE^2 = 1<<24 */
    uint64_t sqrt_scaled = int_sqrt128(sqrt_arg);

    /* fourth_scaled = floor( n^(1/4) * SCALE ) */
    uint64_t fourth_scaled = fourth_root_scaled(n);

    /* difference in scaled units (non‑negative) */
    uint64_t diff_scaled = sqrt_scaled - fourth_scaled;

    /* ceil( diff_scaled / SCALE ) = (diff_scaled + SCALE - 1) >> SHIFT */
    uint64_t ceil_val = (diff_scaled + SCALE - 1) >> SHIFT;
    return (seq_t)ceil_val;
}

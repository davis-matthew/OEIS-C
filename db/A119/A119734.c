/*
 * A119734  a(n) = least k such that 2^k + n is a perfect power, or -1 if no
 *          such k exists.
 * https://oeis.org/A119734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, more, sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 12, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least k ≥ 0 such that 2^k + n is a perfect power
 * (m^e with m ≥ 1, e ≥ 2; 1 is treated as a perfect power for
 * consistency with the OEIS definition), or -1 if no such k exists.
 */
#include <stdbool.h>
#include <stdint.h>

/* power computation with overflow guard: returns 0 if mid^exp > limit */
static uint64_t ipow(uint64_t base, int exp, uint64_t limit)
{
    __int128 res = 1;
    while (exp > 0) {
        res *= base;
        if (res > limit) return 0;        /* overflow marker */
        --exp;
    }
    return (uint64_t)res;
}

/* true iff x is a perfect power (m^e, m ≥ 1, e ≥ 2) */
static bool is_perfect_power_uint64(uint64_t x)
{
    if (x == 1) return true;   /* 1 = 1^e is regarded as perfect power */
    if (x < 4) return false;   /* 0,1,2,3 are not considered here after 1 */

    for (int e = 2; e <= 63; ++e) {
        uint64_t low  = 2;
        uint64_t high = x;
        while (low <= high) {
            uint64_t mid = low + (high - low) / 2;
            uint64_t val = ipow(mid, e, x);
            if (val == x) return true;
            if (val == 0 || val > x) high = mid - 1;
            else low = mid + 1;
        }
    }
    return false;
}

/* public sequence function */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* convert to 64‑bit unsigned safely */

    /* search for the least k with 2^k + n a perfect power */
    for (int k = 0; k <= 63; ++k) {
        uint64_t pow2 = 1ULL << k;      /* 2^k */
        uint64_t val  = pow2 + nn;      /* 2^k + n */

        if (is_perfect_power_uint64(val))
            return (seq_t)k;            /* minimal k found */
    }

    return (seq_t)-1;                    /* no k within the searched range */
}

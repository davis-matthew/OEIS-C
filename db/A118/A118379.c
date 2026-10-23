/*
 * A118379  Natural numbers n which are not perfect power but at least one of
 *          decimal digits of n is a perfect power.
 * https://oeis.org/A118379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   has_perfect_power_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118379: Natural numbers n which are not perfect powers
 * but at least one of decimal digits of n is a perfect power.
 * Computation method: linear search with helper tests for perfect powers
 *   and for presence of a perfect‑power digit (0,1,4,8,9).
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_perfect_power_digit.h"

static inline bool is_perfect_power_uint64(uint64_t n)
{
    if (n <= 1)
        return true;              /* 0 or 1 are perfect powers */
    for (int e = 2; e <= 64; ++e)
    {
        /* Binary search for base b such that b^e == n */
        uint64_t low = 2, high = 2;
        /* Find an upper bound for the root */
        while (true)
        {
            __uint128_t pow = 1;
            for (int i = 0; i < e; ++i)
            {
                pow *= high;
                if (pow > n) break;
            }
            if (pow > n)
                break;
            high *= 2;
            if (high == 0) break;   /* overflow protection */
        }
        /* Now binary search in [low, high] */
        while (low <= high)
        {
            uint64_t mid = low + ((high - low) >> 1);
            __uint128_t pow = 1;
            for (int i = 0; i < e; ++i)
            {
                pow *= mid;
                if (pow > n) break;
            }
            if (pow == n)
                return true;
            if (pow < n)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Function is re‑entrant; no global state needed. */
    idx_t count = 0;
    idx_t candidate = 0;          /* start searching from 1 */
    while (true)
    {
        ++candidate;
        uint64_t cn = (uint64_t)candidate; /* safe for required range */
        if (!is_perfect_power_uint64(cn) && has_perfect_power_digit(cn))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}

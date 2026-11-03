/*
 * A153157  a(n) = A007916(n)^4.
 * https://oeis.org/A153157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007916(n)^4 (the n‑th non‑perfect‑power integer raised
 *            to the 4‑th power)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 233, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 48 terms through n = 9963 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

/* Compute base^exp bound by limit; return >limit if overflow or exceeds limit. */
static inline uint64_t pow_limit_u64(uint64_t base, uint64_t exp, uint64_t limit)
{
    uint64_t result = 1;
    for (uint64_t i = 0; i < exp && result <= limit; ++i) {
        if (base == 0)
            return 0;
        if (result > limit / base)
            return limit + 1;   /* overflow or too large */
        result *= base;
    }
    return result;
}

/* Greatest integer r such that r^e <= x. */
static inline uint64_t int_root_u64(uint64_t x, uint64_t e)
{
    uint64_t low = 1, high = x;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (pow_limit_u64(mid, e, x) <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return floor(log2(x)) + 1, i.e. the bit‑length of x. */
static inline uint64_t bitlen_u64(uint64_t x)
{
    uint64_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Test whether x is a perfect power (exponent >= 2).  1 and primes are not perfect powers. */
static inline bool is_perfect_power_u64(uint64_t x)
{
    if (x < 2)
        return false;   /* 1 is not considered a perfect power here */

    uint64_t maxE = bitlen_u64(x);
    /* exponents e with 2^e <= x => e < maxE */
    for (uint64_t e = 2; e < maxE; ++e) {
        uint64_t r = int_root_u64(x, e);
        if (pow_limit_u64(r, e, x) == x)
            return true;
    }
    return false;
}

/* Compute the n‑th non‑perfect‑power integer.  n is 1‑based. */
static inline seq_t nth_nonperfect_u64(idx_t n)
{
    uint64_t candidate = 2;
    uint64_t count = 0;

    while (count < (uint64_t)n) {
        if (!is_perfect_power_u64(candidate))
            ++count;
        ++candidate;
    }

    return (seq_t)(candidate - 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t base = nth_nonperfect_u64(n);
    return ipow_idx(base, (idx_t)4);
}

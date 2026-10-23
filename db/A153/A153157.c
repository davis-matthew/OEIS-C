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
 * Helpers:   int_root_u64, pow_limit_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/int_root_u64.h"
#include "../utils/pow_limit_u64.h"

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
    return seq_pow(base, (idx_t)4);
}

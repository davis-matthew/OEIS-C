/*
 * A065865  a(n) is the least k such that n*k - 1 and n*k + 1 are both
 *          composite.
 * https://oeis.org/A065865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64_local.h"

/* A065865: a(n) = least k such that both n*k-1 and n*k+1 are composite. */

static inline bool is_composite(uint64_t x)
{
    return (x > 1) && !is_prime_u64_local(x);
}

seq_t a(idx_t n)
{
    uint64_t nn = n;
    for (uint64_t k = 1;; ++k) {
        __uint128_t prod128 = (__uint128_t)nn * k;
        uint64_t prod = (uint64_t)prod128;      /* product fits in 64‑bit */
        uint64_t m1 = prod - 1;
        uint64_t m2 = prod + 1;

        if (is_composite(m1) && is_composite(m2))
            return (seq_t)k;
    }
}

/*
 * A166127  Minimum value of j such that floor(j^2 / prime(n)) > 0.
 * https://oeis.org/A166127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(prime(n))) for n ≥ 2; a(1) = 0
 * Range:     u8 n <= 18, u16 n <= 1763, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the minimal j such that floor(j² / prime(n)) > 0. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
    {
        /* The first term in the OEIS file is 0. */
        return 0;
    }

    /* Retrieve the n-th prime (prime(1) = 2, prime(2) = 3, …). */
    uint32_t p = get_prime(n);

    /* floor_sqrt returns floor(sqrt(p)) as seq_t. */
    seq_t r = floor_sqrt((seq_t)p);

    /* If r*r < p, then the desired ceil(sqrt(p)) is r+1. */
    uint64_t sq = (uint64_t)r * (uint64_t)r;
    if (sq < (uint64_t)p)
    {
        r += 1;
    }
    return r;
}

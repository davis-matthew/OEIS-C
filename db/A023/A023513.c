/*
 * A023513  a(n) = sum of distinct prime divisors of prime(n) + 1.
 * https://oeis.org/A023513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of prime(n)+1
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. If n is 0, return 0. */
    if (n == 0)
        return 0;

    /* Retrieve the nth prime.  get_prime returns a 1‑indexed prime. */
    uint32_t p32 = get_prime(n);
    /* Convert to 64‑bit unsigned for safe addition. */
    uint64_t v = (uint64_t)p32 + 1ULL;

    /* Allocate temporary storage for factors.  For v ≤ 10⁶
       at most 7 distinct primes are possible, so 32 slots is safe. */
    const size_t alloc_sz = 32;
    uint64_t *pr = (uint64_t *)calloc(alloc_sz, sizeof(uint64_t));
    uint64_t *ex = (uint64_t *)calloc(alloc_sz, sizeof(uint64_t));
    size_t cnt = 0;

    /* Factor the number v.  factor_u64 expects pre‑allocated buffers. */
    factor_u64(v, pr, ex, &cnt);

    /* Sum the distinct prime divisors. */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)pr[i];

    /* Clean up. */
    free(pr);
    free(ex);

    return sum;
}

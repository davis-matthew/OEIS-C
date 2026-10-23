/*
 * A023589  a(n) is the number of distinct prime divisors of 2*prime(n)+1.
 * https://oeis.org/A023589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of 2*prime(n)+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/count_distinct_primes.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

static idx_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure the primes array contains at least the first `n` primes. */
static void ensure_primes_upto(idx_t n)
{
    if (primes_cnt >= (size_t)n)
        return;

    /* Compute an upper bound for the nth prime. */
    idx_t limit = _prime_limit_for_idx((idx_t)n);

    idx_t *tmp;
    size_t cnt;
    build_primes(limit, &tmp, &cnt);

    free(primes);          /* discard old array if any */
    primes = tmp;
    primes_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* behaviour below the offset is irrelevant */

    ensure_primes_upto(n);

    /* nth prime (OEIS offset 1). */
    idx_t p = primes[(size_t)n - 1];

    /* Evaluate 2*p + 1. */
    uint64_t val = 2ULL * (uint64_t)p + 1ULL;

    /* Count its distinct prime factors. */
    int cnt = count_distinct_primes(val);

    return (seq_t)cnt;
}

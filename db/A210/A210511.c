/*
 * A210511  Primes formed by concatenating k, k, and 1 for k >= 1.
 * https://oeis.org/A210511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 17, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210511: primes formed by concatenating k, k, and 1 for k ≥ 1 */
/* a(n) is the n-th prime of the form concat(k, k, 1), with n ≥ 1 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/digits.h"
#include "../utils/big_pow.h"

static seq_t *cache = NULL;          /* cached primes */
static size_t cache_cap = 0;         /* capacity of 'cache' */
static idx_t  cache_count = 0;       /* number of primes stored */
static idx_t  next_k = 1;            /* next k to test */

/* Ensure the cache can hold at least 'needed' primes. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: leave cache unchanged. */
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Build the prime candidate for a given k:
 *   value = concat(k, k, 1) = k * 10^(d+1) + k * 10 + 1
 * where d = number of decimal digits of k. */
static seq_t candidate_for_k(idx_t k)
{
    int d = digits(k);                       /* digits of k */
    seq_t pow = big_pow((seq_t)10, (seq_t)(d + 1));
    seq_t part1 = (seq_t)k * pow;            /* k * 10^(d+1) */
    seq_t part2 = (seq_t)k * 10;             /* k * 10      */
    return part1 + part2 + (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0; sequence offset = 1 */

    /* Generate primes until we have at least n of them. */
    while (cache_count < n) {
        seq_t candidate = candidate_for_k(next_k);
        if (is_prime_big(candidate)) {
            ensure_cache_capacity(cache_count + 1);
            cache[cache_count++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}

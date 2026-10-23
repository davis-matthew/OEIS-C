/*
 * A142682  Primes congruent to 28 mod 57.
 * https://oeis.org/A142682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 156, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 28 mod 57:
   a(n) is the nth prime p with p mod 57 == 28. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cached = 0;
static size_t capacity = 0;
static uint64_t last_k = 3; /* 57*3+28 = 199, first prime */

static void grow_cache(size_t needed)
{
    if (capacity >= needed)
        return;
    size_t new_cap = capacity ? capacity * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1 */

    if ((size_t)n <= cached)
        return cache[(size_t)n - 1];

    grow_cache((size_t)n);

    while (cached < (size_t)n) {
        uint64_t cand64 = 57 * last_k + 28;
        seq_t candidate = (seq_t)cand64;
        if (is_prime_big(candidate))
            cache[cached++] = candidate;
        ++last_k;
    }

    return cache[(size_t)n - 1];
}

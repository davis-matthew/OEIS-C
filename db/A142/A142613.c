/*
 * A142613  Primes congruent to 17 mod 55.
 * https://oeis.org/A142613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 17 (mod 55) */
/*  a(1) = 17, a(2) = 127, a(3) = 347, … */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t cache[20000];      /* holds enough terms for n ≤ ~10 000 */
static idx_t cache_len = 0;     /* number of terms stored */
static uint64_t last_candidate = 0;  /* last tested candidate */

/* Ensure the cache holds at least n terms */
static void extend_cache(idx_t n)
{
    while (cache_len < n)
    {
        if (cache_len == 0)
        {
            last_candidate = 17;
        }
        else
        {
            last_candidate += 55;
        }

        if (is_prime((idx_t)last_candidate))
        {
            cache[cache_len++] = (seq_t)last_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, but harmless */

    extend_cache(n);
    return cache[n - 1];
}

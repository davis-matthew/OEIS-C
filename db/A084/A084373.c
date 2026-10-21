/*
 * A084373  Numbers k such that smallest digit of k = total number of divisors
 *          of k.
 * https://oeis.org/A084373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smallest_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate over integers k = 1,2,… and collect those with
   smallest decimal digit equal to the number of divisors of k.
   Results are cached so that subsequent calls can jump directly to
   the stored values.  This yields a sequence exactly as listed
   in OEIS A084373. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smallest_digit.h"

/* Count the number of positive divisors of n.
   The computation stops as soon as the count exceeds 9
   (since the smallest digit cannot exceed 9). */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t cnt = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            cnt *= (uint64_t)(e + 1);
            if (cnt > 9)
                return cnt;
        }
    }
    if (m > 1)
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t current_k = 1;  /* next integer to test */

    /* grow the cache if needed */
    if (cache_len < (size_t)n) {
        if (cache == NULL) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_len < (size_t)n) {
            uint64_t k = current_k;
            ++current_k;

            int min_digit = smallest_digit(k);
            if (min_digit == 0)        /* numbers containing 0 can never match */
                continue;

            uint64_t tau = divisor_count(k);
            if ((uint64_t)min_digit == tau) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)k;
            }
        }
    }

    return cache[(size_t)n - 1];
}

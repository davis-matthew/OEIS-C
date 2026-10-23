/*
 * A028834  Numbers whose sum of digits is a prime.
 * https://oeis.org/A028834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 9380, u32 n <= 9380, u64 n <= 9380,
 *            u128 n <= 9380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose sum of decimal digits is a prime (offset 1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute the sum of the decimal digits of v. */
static seq_t digit_sum(seq_t v)
{
    seq_t sum = (seq_t)0;
    while (v > (seq_t)0) {
        sum += v % (seq_t)10;
        v /= (seq_t)10;
    }
    return sum;
}

/* Very small prime test, safe for all values that fit in unsigned long long. */
static bool is_prime_small(unsigned long long n)
{
    if (n < 2ULL) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (unsigned long long d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* Cache for the generated sequence.  It grows as needed. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = (seq_t)2;   /* start checking at 2 */

/* Ensure cache has room for at least new_cap elements.  Uses malloc/realloc. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* abort on allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n elements. */
    while (cache_size < (size_t)n) {
        seq_t val     = next_candidate;
        next_candidate += (seq_t)1;

        seq_t sum = digit_sum(val);
        if (is_prime_small((unsigned long long)sum)) {
            /* Grow cache if necessary. */
            if (cache_size == cache_cap)
                ensure_cache_capacity((cache_cap == 0) ? 16 : cache_cap * 2);
            cache[cache_size++] = val;
        }
    }

    /* Return the requested term (1‑based indexing). */
    return cache[(size_t)(n - 1)];
}

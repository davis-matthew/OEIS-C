/*
 * A065079  Primes > 3 for which the alternating bit sum (A065359) is not
 *          equal to 1 or 2.
 * https://oeis.org/A065079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 949 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A065079: primes > 3 whose alternating binary digit sum
   (starting with + on the least‑significant bit) is neither 1 nor 2. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Alternating binary digit sum.   LSB has sign +1, next sign –1, etc. */
static int alt_bit_sum(seq_t v)
{
    int sum = 0;
    int sign = 1;
    while (v != (seq_t)0) {
        if ((v & (seq_t)1) != (seq_t)0)
            sum += sign;
        sign = -sign;
        v >>= 1;
    }
    return sum;
}

/* Cache of sequence values.  Allocated lazily and grown as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;      /* number of cached terms */

/* Next odd integer to test for primality.  Starts at 5 (>3). */
static uint64_t next_candidate = 5;

/* Grow cache capacity to at least `min_cap` entries. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < min_cap)
        new_cap = min_cap;
    cache = seq_realloc(cache, cache_cap, new_cap);
    if (cache == NULL)            /* allocation failure – abort */
        abort();
    cache_cap = new_cap;
}

/* Generate and cache sequence terms until at least `n` terms are stored. */
static void ensure_terms(idx_t n)
{
    size_t target = (size_t)n;
    if (cache_len >= target)
        return;

    while (cache_len < target) {
        /* Find next prime > 3. */
        while (!is_prime((idx_t)next_candidate))
            ++next_candidate;

        /* Check alternating bit sum condition. */
        int s = alt_bit_sum((seq_t)next_candidate);
        if (s != 1 && s != 2) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Public interface required by the build system.  a(1) = 11. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1 – unspecified for n=0 */
        return (seq_t)0;
    ensure_terms(n);
    return cache[n - 1];
}

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
 * Helpers:   alt_bit_sum, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A065079: primes > 3 whose alternating binary digit sum
   (starting with + on the least‑significant bit) is neither 1 nor 2. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t)      */
#include "../utils/seq_realloc.h"  /* seq_t* seq_realloc(seq_t*,size_t,size_t) */
#include "../utils/alt_bit_sum.h"

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

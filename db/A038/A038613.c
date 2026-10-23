/*
 * A038613  Primes not containing the digit '5'.
 * https://oeis.org/A038613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 4093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_digit_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A038613: primes that do not contain the digit '5'.
   The function performs an incremental search, caching primes found so far.
   Calls can be made in any order; the cache is automatically rebuilt to reach
   the requested index. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/no_digit_five.h"

/* Helper: test whether a 64‑bit integer is prime.
   This simple deterministic trial‑division test is sufficient for all
   candidates that occur for n <= 10 000 (the 10 000th term is far below 1 e6). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;
    for (uint64_t d = 5; d * d <= n; d += 6) {
        if (n % d == 0)   return false;
        if (n % (d + 2) == 0) return false;
    }
    return true;
}

/* Cache of primes found so far. */
static seq_t   *cache      = NULL;   /* pointer to array of seq_t */
static size_t   cache_len  = 0;      /* number of primes stored */
static size_t   cache_cap  = 0;      /* capacity of the array */

/* The largest integer that has already been examined.  Starts at 1 so that
   the search begins with 2. */
static idx_t   last_checked = 1;

/* Ensure the cache has capacity for at least 'need' primes. */
static void cache_ensure(size_t need)
{
    if (need <= cache_cap)  return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least 'target' primes. */
static void cache_fill_to(idx_t target)
{
    if (cache_len >= (size_t)target) return;
    cache_ensure((size_t)target);
    uint64_t candidate = (uint64_t)last_checked + 1;
    while (cache_len < (size_t)target) {
        if (no_digit_five(candidate) && is_prime_uint64(candidate)) {
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    last_checked = (idx_t)(candidate - 1);
}

/* External linkage as required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 as guard */

    /* Expand the cache to include at least n primes. */
    cache_fill_to(n);

    /* Return the (n‑1)th stored prime. */
    return cache[n - 1];
}

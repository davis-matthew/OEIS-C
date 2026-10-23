/*
 * A158277  The lesser of twin prime pairs with each prime in a different
 *          century.
 * https://oeis.org/A158277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the lower twin prime p such that p % 100 == 99 and p + 2
 *            is prime
 * Range:     u8 none, u16 n <= 19, u32 n <= 26, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches sequentially for twin prime pairs (p, p+2) where
   the lower prime ends in 99.  The values are cached so that repeated calls
   are efficient.  The function uses the provided is_prime_big() routine
   for primality testing, which works for both fixed‑size and arbitrary
   precision integer types. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached a(n) values */
static size_t cache_cnt = 0, cache_cap = 0;

static seq_t next_candidate = 599;   /* next number ending in 99 to test */

/* Append a new term to the cache, allocating more space if necessary. */
static void push_cache(seq_t v) {
    if (cache_cnt >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = v;
}

/* Generate terms until the cache has at least n entries. */
static void ensure_cache(idx_t n) {
    while (cache_cnt < n) {
        seq_t cand = next_candidate;
        if (is_prime_big(cand) && is_prime_big(cand + 2)) {
            push_cache(cand);
        }
        next_candidate += 100;   /* move to the next number ending in 99 */
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                /* undefined for n < 1, but safe return */
    ensure_cache(n);
    return cache[n - 1];
}

/*
 * A165801  f(n), f(f(n)), ... are all prime, where f(n) = (n-1)/2. Stop when
 *          f(...f(n)...) is less than 4.
 * https://oeis.org/A165801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A165801 – numbers n such that repeatedly applying
   f(n) = (n-1)/2 yields primes until the result falls below 4.
   The algorithm enumerates integers, skipping even numbers > 2,
   and tests the chain for primality. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Test whether a candidate satisfies the property.
   Even integers > 2 are not allowed. */
static bool good_candidate(seq_t cand) {
    /* 1 and 2 are always included. */
    if (cand == (seq_t)1 || cand == (seq_t)2)
        return true;

    /* Even numbers greater than 2 are forbidden. */
    if ((cand & (seq_t)1) == (seq_t)0)
        return false;

    /* Now cand is odd and ≥ 3.  Check the iterated f(n). */
    seq_t cur = cand;
    /* We stop when cur becomes < 4; the value < 4 need not be prime. */
    while (cur >= (seq_t)4) {
        seq_t next = (cur - (seq_t)1) >> 1; /* (cur-1)/2, cur is odd */
        if (!is_prime_big(next))
            return false;
        cur = next;
    }
    return true; /* all qualifying intermediate values were prime */
}

/* Cache storage for the computed sequence. */
static seq_t *a_cache      = NULL; /* array of terms */
static size_t a_cache_cap  = 0;     /* allocated capacity */
static size_t a_cache_len  = 0;     /* number of already stored terms */
static idx_t  last_checked = 0;     /* last integer examined */

/* Append a new term to the cache, growing the array if needed. */
static void cache_append(seq_t val) {
    if (a_cache_len == a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache     = tmp;
        a_cache_cap = new_cap;
    }
    a_cache[a_cache_len++] = val;
}

/* Public function requested by the exercise. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for 0, but return 0 */

    /* Generate terms until the cache contains at least n terms. */
    while (a_cache_len < n) {
        last_checked++;                    /* next integer to test   */
        seq_t cand = (seq_t)last_checked;
        if (good_candidate(cand))
            cache_append(cand);
    }
    return a_cache[n - 1];
}

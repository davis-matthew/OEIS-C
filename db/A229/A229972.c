/*
 * A229972  Nonprime numbers whose product of proper divisors is a perfect
 *          cube.
 * https://oeis.org/A229972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 8410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, floor_cube_root, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A229972 (nonprime numbers whose product of proper divisors is a perfect cube).  
   This implementation searches integers in increasing order, caching the results
   so that calls for any index n are answered in O(n) time overall. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_cube_root.h"
#include "../utils/divisor_count_u64.h"

/* static cache for computed terms */
static seq_t *term_cache = NULL;
static size_t term_cache_len = 0;
static size_t term_cache_cap = 0;
static idx_t last_checked = 1;  /* next integer to test */

/* append a value to the cache, expanding it as necessary */
static void append_term(seq_t v)
{
    if (term_cache_len == term_cache_cap) {
        size_t new_cap = term_cache_cap ? term_cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        term_cache = tmp;
        term_cache_cap = new_cap;
    }
    term_cache[term_cache_len++] = v;
}

/* test whether a value m is a perfect cube (using seq_t arithmetic) */
static bool is_cube(idx_t m)
{
    seq_t x = (seq_t)m;
    seq_t c = floor_cube_root(x);
    return c * c * c == x;
}

/* main sequence function */
seq_t a(idx_t n)
{
    while (term_cache_len < n) {
        idx_t m = last_checked++;               /* integer to test */
        if (m == 1) {                           /* 1 is included explicitly */
            append_term((seq_t)m);
            continue;
        }
        if (is_prime(m)) continue;               /* skip primes */
        if (is_cube(m) || (divisor_count_u64((uint64_t)m) % 3 == 2))
            append_term((seq_t)m);
    }
    return term_cache[n - 1];
}

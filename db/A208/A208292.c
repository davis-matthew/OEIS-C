/*
 * A208292  Primes of the form (n^2+1)/26.
 * https://oeis.org/A208292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 32, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the n-th prime of the form (m^2+1)/26 where m is an odd integer.
 * The sequence is the ordered list of primes of the form (m^2+1)/26 (m odd).
 * The function a(1) returns the first such prime, a(2) the second, etc.
 * The function uses caching so calls may be in any order.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* array of found primes */
static size_t cache_len = 0;     /* number of stored primes */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_m = 1;         /* next odd m to test */

/* double cache capacity when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – in this context we simply ignore further growth;
           the build system is expected to have sufficient memory */
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* retrieve the n-th prime of the required form (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined – avoid underflow */

    /* find primes until we have at least n of them */
    while (cache_len < n) {
        if (!cache || cache_len == cache_cap)
            grow_cache();

        /* compute (m^2+1)/26 for current odd m */
        seq_t m = (seq_t)next_m;
        seq_t num = m * m + 1;
        if (num % 26 == 0) {
            seq_t p = num / 26;
            if (is_prime_big(p))
                cache[cache_len++] = p;
        }
        next_m += 2;          /* next odd integer */
    }
    return cache[n - 1];
}

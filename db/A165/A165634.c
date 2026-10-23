/*
 * A165634  Start with x=1 and repeat: if x is a prime number then (append i
 *          and then x, with x=prime(i)) else (only append x), continue with
 *          x:=x+1.
 * https://oeis.org/A165634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165634:  start with x = 1 and for each natural number x
 *   if x is prime, append its prime index i and then x
 *   otherwise append only x.
 * The sequence is built incrementally; only the first two terms of
 * a prime are inserted, the rest (composite numbers) just once.
 * All values are stored in a cache so that any call a(n) returns
 * the sequence term at position n (1‑based), even if calls are
 * not in increasing order.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Ensure the cache has capacity for at least 'needed' entries. */
static void
ensure_cache_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap) ? (*cap) << 1 : 1;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* 1‑based index, but allow n==0 by returning 0 (undefined). */
    if (n == 0) return 0;

    /* static state for memoisation */
    static idx_t cur_x      = 0;  /* last natural number processed */
    static idx_t prime_cnt  = 0;  /* number of primes seen so far */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        cur_x++;                   /* next natural number */

        if (is_prime(cur_x)) {     /* x is prime */
            prime_cnt++;           /* its prime index */

            ensure_cache_capacity(cache_len + 2, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)prime_cnt;   /* append i */
            cache[cache_len++] = (seq_t)cur_x;       /* append x */
        } else {                   /* composite or 1 */
            ensure_cache_capacity(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)cur_x;       /* append x only */
        }
    }

    return cache[n - 1];
}

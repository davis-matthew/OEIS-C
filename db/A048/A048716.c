/*
 * A048716  Numbers k such that (k AND 4*k) = 0, where AND is bitwise-and.
 * https://oeis.org/A048716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 3025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nth term of A048716 (Numbers k such that (k AND 4*k)=0).
   Each term is produced by checking the condition (k & (k << 2)) == 0
   while walking through the non‑negative integers. The function keeps
   a static cache to allow repeated calls in any order. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_cap = 0;     /* current capacity of cache   */
static idx_t  cache_len = 0;     /* number of terms cached      */
static idx_t  next_candidate = 0;/* next integer to test        */

/* Ensure the cache contains at least 'n' terms */
static void ensure_terms(idx_t n)
{
    /* Lazy initialization */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1); /* abort on allocation failure */
        /* add the first term, k = 0 */
        cache[cache_len++] = (seq_t)0;
        next_candidate = 1;
    }

    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;
        /* test (k AND 4*k) == 0   <=>  (k & (k << 2)) == 0 */
        if ( (val & (val << 2)) == 0 ) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based; a(1) = 0 */
    if (n == 0) return (seq_t)0;  /* safeguard, though n>=1 per spec */
    ensure_terms(n);
    return cache[n - 1];
}

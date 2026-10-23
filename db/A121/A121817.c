/*
 * A121817  Numbers m such that 23 + 36*m*(m+1) is prime.
 * https://oeis.org/A121817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m such that 23 + 36*m*(m+1) is prime
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 13, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
/* Prime test for arbitrary‑precision seq_t values. */
#include "../utils/is_prime_big.h"

/* Cache of found m values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked_m = 0;   /* next m to test */

/* Grow the cache to accommodate at least 'need' terms. */
static void extend_cache_until(idx_t need)
{
    while (cache_len < need) {
        seq_t m = (seq_t)last_checked_m;
        /* Compute 23 + 36*m*(m+1) using seq_t arithmetic. */
        seq_t val = (seq_t)36 * m * (m + (seq_t)1) + (seq_t)23;
        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Critical memory failure – terminate. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        ++last_checked_m;
    }
}

/* Return the n-th term of the sequence (1‑based).  Undefined for n<1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* guard for misuse */
    extend_cache_until(n);
    return cache[n - 1];
}

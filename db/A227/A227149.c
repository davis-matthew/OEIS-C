/*
 * A227149  Numbers k for which the sum of digits is odd when k is written in
 *          the factorial base (A007623).
 * https://oeis.org/A227149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 2520, u32 n <= 2520, u64 n <= 2520,
 *            u128 n <= 2520, big any n
 * Helpers:   odd_factorial_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A227149 – numbers k whose factorial-base digit sum is odd.
 * The function implements a search algorithm: it scans integers k
 * and collects those that satisfy the property until the desired
 * index n is reached.  Because the sequence has density ½, the
 * k value needed is only on the order of 2n, so the algorithm
 * runs quickly for n up to 10000.  Results are cached statically
 * so repeated calls (in any order) are handled efficiently.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_factorial_digit_sum.h"

/* Cache for already computed terms */
static seq_t *cache   = NULL;  /* array of results a(1)…a(cache_len) */
static size_t cache_cap = 0;   /* allocated capacity of cache */
static size_t cache_len = 0;   /* number of terms stored (>=0) */

/* Next integer candidate to test, starting from 1 */
static seq_t next_k = 1;

/* Expand cache storage if needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* a(1) is the first element; i.e., n starts at 1 */
    if (n == 0) {
        /* Undefined for n==0; return 0 as a safe placeholder */
        return (seq_t)0;
    }

    /* Extend cache up to index n */
    while (cache_len < (size_t)n) {
        if (odd_factorial_digit_sum(next_k)) {
            if (cache_len == cache_cap) {
                grow_cache();
            }
            cache[cache_len] = next_k;
            cache_len++;
        }
        next_k++;
    }

    return cache[(size_t)(n - 1)];
}

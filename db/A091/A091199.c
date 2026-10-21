/*
 * A091199  Numbers k such that (6k-3)^2 + 2 is prime.
 * https://oeis.org/A091199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer k such that (6k‑3)² + 2 is prime, found by incremental search. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache of discovered k-values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t search_k = 1;  /* next k to test */

/* Return the n-th term (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 1, but return 0 for safety */
        return 0;

    /* Ensure we have at least n cached values. */
    while (cache_len < n) {
        /* Compute m = 6*k-3 and p = m^2 + 2 using 64‑bit arithmetic. */
        uint64_t k = (uint64_t)search_k;
        uint64_t m = 6ULL * k - 3ULL;
        uint64_t p = m * m + 2ULL;

        if (is_prime((idx_t)p)) {
            /* Grow cache if necessary. */
            if (cache_cap == cache_len) {
                size_t newcap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)search_k;
        }
        ++search_k;
    }
    return cache[n - 1];
}

/*
 * A138643  Nonnegative integers k such that 19*k-3 is prime.
 * https://oeis.org/A138643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 322, u32 n <= 323, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth positive integer k such that 19*k-3 is prime
   (OEIS A138643). The function generates terms on demand and caches
   previously found values to allow arbitrary order of calls. */
#include <stdlib.h>   /* for realloc, EXIT_FAILURE */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found k's */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    static idx_t last_k = 0;               /* last tested integer */

    if (n == 0)              /* offset is 1; 0 is unused */
        return 0;

    /* Generate terms until we have at least n cached */
    while (cache_len < n) {
        ++last_k;
        seq_t val = (seq_t)19 * (seq_t)last_k - (seq_t)3;
        if (is_prime_big(val)) {
            /* Grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In a real environment we could handle this differently,
                       but exit is acceptable here because this should never happen. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_k;
        }
    }
    return cache[n - 1];
}

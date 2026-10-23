/*
 * A028840  Numbers k such that sum of digits of k is a Fibonacci number.
 * https://oeis.org/A028840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Enumerate integers k whose digit sum is a Fibonacci number. a(1)
 *            = 0, a(2) = 1, a(3) = 2, a(4) = 3, ... (Offset 1.)
 * Range:     u8 n <= 73, u16 n <= 7303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_fibo_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/is_fibo_sum.h"

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static idx_t next_k = 0;      /* next integer to test for inclusion */

    /* Grow cache to at least n elements. */
    if (cache_len < n) {
        /* Allocate or resize the cache. */
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        }

        /* Scan new numbers until we have n terms. */
        idx_t k = next_k;
        while (cache_len < n) {
            seq_t sum = digit_sum((seq_t)k);
            if (is_fibo_sum(sum)) {
                cache[cache_len++] = (seq_t)k;
            }
            ++k;
        }
        next_k = k;  /* update next starting point */
    }

    return cache[n - 1];   /* offset 1 -> index n-1 in 0‑based array */
}

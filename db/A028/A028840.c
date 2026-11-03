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
 * Validated: standard, pass, 77 terms through n = 9850 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* Helper: true iff s is a Fibonacci number (including 0). */
static bool is_fibo_sum(seq_t s)
{
    if (s == 0) return true;
    seq_t a = 0, b = 1;
    while (b < s) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b == s;
}

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
            cache = mem_new(seq_t, (size_t)n);
        } else {
            cache = mem_resize(seq_t, cache, (size_t)n);
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

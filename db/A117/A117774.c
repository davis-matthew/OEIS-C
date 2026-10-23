/*
 * A117774  Fibonacci numbers which are divisible by the sum of their digits.
 * https://oeis.org/A117774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation: iterate Fibonacci numbers, keep those divisible by the sum of
   their decimal digits, storing each value only once (duplicates are
   omitted).  This implements the definition of OEIS A117774.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is 1‑indexed; undefined for n=0. */
        return 0;
    }

    /* Static cache to store the discovered sequence terms. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Fibonacci state: f_prev = F_{k-1}, f_curr = F_k. */
    static seq_t f_prev = 0;
    static seq_t f_curr = 1;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        /* Nothing else to do; the first test will be for F_1 = 1. */
    }

    /* Generate Fibonacci numbers until we have at least n valid entries. */
    while (cache_len < (size_t)n) {
        /* Test current Fibonacci number. */
        seq_t ds = digit_sum(f_curr);
        if (ds != 0 && (f_curr % ds) == 0) {
            /* Avoid storing duplicate values (only 1 occurs twice). */
            if (cache_len == 0 || f_curr != cache[cache_len - 1]) {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = f_curr;
            }
        }

        /* Advance to the next Fibonacci number, checking for overflow. */
        seq_t next = f_prev + f_curr;
        if (next < f_curr) {            /* overflow detected */
            break;
        }
        f_prev = f_curr;
        f_curr = next;
    }

    /* By now cache_len >= n (or we ran out of representable Fibonacci
       numbers); the required term is guaranteed to be cached. */
    return cache[n - 1];
}

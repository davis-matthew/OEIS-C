/*
 * A098731  Numbers k such that 2*R_k is a happy number (A007770), where R_k =
 *          11...1 is the repunit (A002275) of length k.
 * https://oeis.org/A098731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   A098731 – Numbers k such that 2·R_k is a happy number. Here R_k
 *            = 11…1 (k digits). 2·R_k is the decimal string of k digits '2'.
 *            The sum of squares of its digits is 4·k, so the happy test start
 *            value is 4·k. After one or two reductions the values are < 2000,
 *            hence a small visited array suffices
 * Range:     u8 n <= 42, u16 n <= 9778, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_square_sum_uint64, is_happy_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_square_sum_uint64.h"
#include "../utils/is_happy_uint64.h"

/* Static cache for previously found terms. */
static seq_t *k_cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static idx_t  last_k    = 0;

/* Return the n-th term (1‑based) of A098731. */
seq_t a(idx_t n)
{
    /* Grow cache until it contains at least n terms. */
    while (cache_sz < (size_t)n) {
        ++last_k;                                 /* next k to test */

        if (is_happy_uint64(4 * (uint64_t)last_k)) { /* 2*R_k is happy? */
            /* Ensure enough storage. */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                 /* allocation failure */
                k_cache = tmp;
                cache_cap = new_cap;
            }
            k_cache[cache_sz++] = (seq_t)last_k;  /* store the term */
        }
    }

    return k_cache[n - 1];
}

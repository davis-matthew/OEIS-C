/*
 * A058369  Numbers k such that k and k^2 have same digit sum.
 * https://oeis.org/A058369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 1056, u64 n <= 8354,
 *            u128 n <= 8354, big any n
 * Validated: standard, pass, 70 terms through n = 7738 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find all integers k such that digit_sum(k) == digit_sum(k^2).
 * a(n) returns the n‑th such integer (1‑indexed). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Helper: digit sum of k^2 computed by multiplying k by itself. */
static seq_t digit_sum_of_square(seq_t k)
{
    seq_t sq = seq_mul(k, k);          /* k * k */
    seq_t sum = digit_sum(sq);         /* digit sum of the square */
    return sum;
}

/* Return the n‑th term of A058369. */
seq_t a(idx_t n)
{
    /* Static memoisation cache (1‑indexed). */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* current capacity of cache array */
    static idx_t next_k = 0;       /* next integer to test */

    /* Generate terms until we have n of them. */
    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t ds_k  = digit_sum(k);
        seq_t ds_k2 = digit_sum_of_square(k);
        if (ds_k == ds_k2) {
            append_candidate(&cache, &cache_len, &cache_cap, k);
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}

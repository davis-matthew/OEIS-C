/*
 * A020440  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly four 1's.
 * https://oeis.org/A020440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2962, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9808 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the nth integer k whose continued‑fraction
   period for sqrt(k) contains exactly four terms equal to 1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool has_four_ones(seq_t k, seq_t a0)
{
    /* Skip perfect squares */
    if ((a0 * a0) == k)
        return false;

    seq_t two_a0 = a0 + a0;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    size_t ones = 0;

    while (true) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        if (a == 1) {
            if (++ones > 4)
                return false;          /* more than four 1's, stop early */
        }
        if (a == two_a0)
            break;                     /* end of period */
    }
    return ones == 4;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cached sequence values   */
    static size_t cache_len = 0;      /* number of cached values  */
    static size_t cache_cap = 0;      /* allocated capacity       */
    static seq_t next_k     = 2;      /* next integer to test     */
    static seq_t current_a0 = 1;      /* floor(sqrt(next_k))      */
    static seq_t next_square = 4;     /* (current_a0+1)^2         */
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 64;
        cache     = seq_alloc(cache_cap);
        cache_len = 0;
        initialized = true;
    }

    /* generate terms until we have at least n values cached */
    while (cache_len < (size_t)n) {
        /* increment floor(sqrt) whenever next_k reaches a new square */
        if (next_k >= next_square) {
            current_a0 = current_a0 + 1;
            next_square = (current_a0 + 1) * (current_a0 + 1);
        }

        if (has_four_ones(next_k, current_a0)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }
    return cache[(size_t)n - 1];
}

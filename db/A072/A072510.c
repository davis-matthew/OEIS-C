/*
 * A072510  Numbers n with property that n = product of first k divisors of n
 *          for some k.
 * https://oeis.org/A072510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 7814 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072510: numbers n such that n equals the product of the first k divisors
   of n for some k. The function searches sequentially, caching found terms. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Small insertion sort for an array of idx_t (max 128 elements). */
static void sort_ul(idx_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        idx_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j-1] > key) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }
}

/* Test whether x satisfies the divisor-product property. */
static bool has_property(seq_t x)
{
    idx_t divs[128];
    size_t m = 0;

    /* Enumerate divisors up to sqrt(x). */
    for (idx_t i = 1; (seq_t)i * (seq_t)i <= x; ++i) {
        if (x % (seq_t)i == 0) {
            divs[m++] = i;
            seq_t big = x / (seq_t)i;
            if ((seq_t)i != big) {
                divs[m++] = (idx_t)big;
            }
        }
    }

    sort_ul(divs, m);

    seq_t prod = 1;
    for (size_t j = 0; j < m; ++j) {
        prod *= (seq_t)divs[j];
        if (prod == x) return true;
        if (prod > x) return false;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but harmless */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t seen_count = 0;
    static idx_t next_candidate = 1;

    while (seen_count < n) {
        if (cache == NULL || seen_count >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = alloc_seq_array(new_cap);
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        if (has_property((seq_t)next_candidate)) {
            cache[seen_count] = (seq_t)next_candidate;
            ++seen_count;
        }
        ++next_candidate;
    }

    return cache[n-1];
}

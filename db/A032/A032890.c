/*
 * A032890  Numbers whose base-2 representation Sum_{i=0..m} d(i)*2^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   zigzag_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032890: numbers whose binary representation alternates
   d0 >= d1 <= d2 >= d3 ... (zigzag).  This implementation
   generates the nth term by counting up to the needed position
   and caching past results for subsequent calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/zigzag_valid.h"

static seq_t *cache = NULL;    /* cached terms */
static size_t cache_cap = 0;   /* current allocation size */
static idx_t cache_len = 0;    /* number of terms cached */
static seq_t last_candidate = 1; /* next value to test */

/* Ensure the cache contains at least the first n terms. */
static void extend_cache_to(idx_t n)
{
    if (cache_len >= n) return;
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        last_candidate = 1;
    }
    while (cache_len < n) {
        if (zigzag_valid(last_candidate)) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = last_candidate;
        }
        ++last_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is defined from n=1 */
    extend_cache_to(n);
    return cache[n-1];
}

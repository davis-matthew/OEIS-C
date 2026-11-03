/*
 * A219248  Numbers such that the absolute difference of any two adjacent
 *          (decimal) digits is prime.
 * https://oeis.org/A219248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 3816, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9045 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th number whose absolute difference of any two adjacent
 * decimal digits is a prime (2, 3, 5 or 7).  The sequence is generated
 * by brute‑force search and cached for arbitrary‑order queries. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static bool is_valid(seq_t x)
{
    if (x < 10) return true;            /* single digits are always valid */
    while (x >= 10) {
        seq_t curr  = x % 10;
        seq_t next  = (x / 10) % 10;
        seq_t diff  = (curr > next) ? curr - next : next - curr;
        if (diff != 2 && diff != 3 && diff != 5 && diff != 7)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cached terms */
    static size_t cache_cap = 0;      /* cached capacity */
    static size_t cache_len = 0;      /* number of cached terms */
    static seq_t next_num   = 0;      /* next integer to test */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        if (is_valid(next_num)) {
            if (cache_len == cache_cap)           /* grow the cache */
                grow_seq_array(&cache, &cache_cap, cache_cap * 2);
            cache[cache_len++] = next_num;
        }
        ++next_num;
    }

    return cache[(size_t)(n - 1)];
}

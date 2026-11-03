/*
 * A069708  Triangular numbers with property that swapping first and last
 *          digits also gives a triangular number.
 * https://oeis.org/A069708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 970 (2026-11-03)
 * Helpers:   is_triangular, isqrt_u128_to_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th triangular number T_k such that swapping its first
   and last decimal digits yields another triangular number. The function
   generates triangular numbers sequentially and stores those satisfying the
   property. It caches computed terms so that calls with arbitrary n are
   efficient. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_triangular.h"
#include "../utils/isqrt_u128_to_u64.h"
#include "../utils/mem.h"

/* Ensure that the cache has room for at least 'need' elements. */
static inline void ensure_cache_cap(size_t *cap, size_t need, seq_t **arr)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need)
            new_cap = need;
        seq_t *new_arr = mem_resize(seq_t, *arr, new_cap);
        if (!new_arr)
            exit(1); /* Allocation failed */
        *arr = new_arr;
        *cap = new_cap;
    }
}

/* Compute 10^e for small e (e <= 18). */
static inline uint64_t pow10_uint32(int e)
{
    uint64_t r = 1;
    while (e-- > 0)
        r *= 10;
    return r;
}

/* Swap the first and last decimal digits of a 64‑bit unsigned integer. */
static inline uint64_t swap_first_last_uint64(uint64_t v)
{
    if (v < 10)
        return v;
    int len = 0;
    uint64_t tmp = v;
    while (tmp > 0) {
        len++;
        tmp /= 10;
    }
    uint64_t base = pow10_uint32(len - 1);      /* 10^(len-1) */
    uint64_t first = v / base;
    uint64_t last  = v % 10;
    uint64_t middle = (v % base) / 10;
    return last * base + middle * 10 + first;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;          /* k for next triangular number T_k */

    while (cache_len < (size_t)n) {
        /* Compute the next triangular number T_k */
        seq_t T = (seq_t)next_k * ((seq_t)next_k + 1) / 2;

        /* Convert to uint64_t for digit manipulation */
        uint64_t T64 = (uint64_t)T;
        uint64_t swapped = swap_first_last_uint64(T64);

        /* If swapping gives another triangular number, store T */
        if (is_triangular(swapped)) {
            ensure_cache_cap(&cache_cap, cache_len + 1, &cache);
            cache[cache_len++] = T;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}

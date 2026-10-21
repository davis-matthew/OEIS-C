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
 * Helpers:   ensure_cache_cap, floor_sqrt_128, is_triangular_uint64,
 *            pow10_uint32, swap_first_last_uint64
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
#include "../utils/ensure_cache_cap.h"
#include "../utils/floor_sqrt_128.h"
#include "../utils/is_triangular_uint64.h"
#include "../utils/pow10_uint32.h"
#include "../utils/swap_first_last_uint64.h"

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
        if (is_triangular_uint64(swapped)) {
            ensure_cache_cap(&cache_cap, cache_len + 1, &cache);
            cache[cache_len++] = T;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}

/*
 * A085575  Squares whose sum of nonsquare digits is a square.
 * https://oeis.org/A085575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 7388, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_square, is_square_int, nonsquare_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements OEIS sequence A085575.
 * There is no known closed formula; the implementation
 * simply tests successive perfect squares in increasing order
 * and keeps those for which the sum of the non‑square decimal
 * digits (digits 0,1,4,9 excluded) itself is a perfect square.
 * The function memoises already found terms so that calls
 * with arbitrary n are efficient.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_square.h"
#include "../utils/is_square_int.h"
#include "../utils/nonsquare_digit_sum.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_cap = 0;     /* capacity of cache */
static idx_t  cache_cnt = 0;     /* number of cached terms */
static idx_t  next_k = 0;        /* next integer to square */

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms computed */
    while (cache_cnt < n) {
        /* Compute next square */
        seq_t sq = (seq_t)next_k * (seq_t)next_k;

        if (good_square(sq)) {
            /* Grow cache if necessary */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failed: return 0 as a safe fallback */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = sq;
        }
        ++next_k;
    }
    return cache[n - 1];
}

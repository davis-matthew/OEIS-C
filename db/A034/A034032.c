/*
 * A034032  Imprimitively but not primitively represented by x^2+2y^2.
 * https://oeis.org/A034032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = the n‑th integer that is representable by
 *  x^2 + 2y^2 but has no primitive representation gcd(x,y)=1.
 *  The sequence starts  a(1)=0, a(2)=4, a(3)=8, …
 */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* integer square root of a uint64_t (floor) */
static uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t low = 0, high = v;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * mid <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Euclidean gcd for uint64_t */

/* dynamic array that contains the sequence values */
static seq_t *seq_list = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t cur_limit = 0;

/* append a value to the sequence */
static void append_value(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_list = tmp;
        seq_cap  = new_cap;
    }
    seq_list[seq_len++] = v;
}

/* recompute the sequence up to numeric limit */
static void recompute_upto(uint64_t limit)
{
    /* mark all numbers up to limit */
    uint8_t *rep  = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    uint8_t *prim = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!rep || !prim) exit(EXIT_FAILURE);

    uint64_t x_max = isqrt_uint64(limit);
    for (uint64_t x = 0; x <= x_max; ++x) {
        uint64_t x2 = x * x;
        if (x2 > limit) break;
        uint64_t rem   = limit - x2;
        uint64_t y_max = isqrt_uint64(rem / 2);
        for (uint64_t y = 0; y <= y_max; ++y) {
            uint64_t n = x2 + 2 * y * y;
            if (n > limit) continue;
            rep[n]  = 1;
            if (gcd_uint64(x, y) == 1)
                prim[n] = 1;
        }
    }

    /* rebuild the sequence array */
    free(seq_list);
    seq_list = NULL;
    seq_len  = 0;
    seq_cap  = 0;

    append_value((seq_t)0);                 /* a(1) = 0 */
    for (uint64_t t = 1; t <= limit; ++t) {
        if (rep[t] && !prim[t])
            append_value((seq_t)t);
    }

    free(rep);
    free(prim);
    cur_limit = limit;
}

/* ensure we have at least n_index terms in the sequence */
static void ensure_terms(idx_t n_index)
{
    if (seq_len >= (size_t)n_index) return;
    uint64_t limit = cur_limit ? cur_limit : 128;
    while (seq_len < (size_t)n_index) {
        recompute_upto(limit);
        if (seq_len >= (size_t)n_index) break;
        limit *= 2;
    }
}

/* public function as required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but safe */
    ensure_terms(n);
    return seq_list[n - 1];
}

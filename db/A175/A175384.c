/*
 * A175384  A positive integer k is included if the largest square dividing k
 *          is not equal to the largest square that, when written in binary,
 *          occurs as a substring in binary k.
 * https://oeis.org/A175384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth positive integer k for which * the largest
 *            square dividing k is not equal to the largest * square occurring
 *            as a binary substring of k
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   _floor_isqrt_u64, bits_contained, floor_log2_u64,
 *            largest_square_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/floor_log2_u64.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/bits_contained.h"
#include "../utils/largest_square_div.h"

/* Cache for previously found terms. */
static seq_t   *term_cache = NULL;
static size_t  term_cap   = 0;
static size_t  term_len   = 0;
static uint64_t last_checked = 0ULL;

/* Largest square whose binary representation occurs in that of k. */
static uint64_t largest_square_in_binary(uint64_t k)
{
    uint8_t k_len = (uint8_t)(floor_log2_u64((idx_t)k) + 1);

    uint64_t max_val;
    if (k_len == 64)
        max_val = UINT64_MAX;
    else
        max_val = ((uint64_t)1 << k_len);

    uint64_t s_max = _floor_isqrt_u64(max_val - 1ULL);

    for (uint64_t s = s_max; s > 0; --s) {
        uint64_t sq = s * s;
        uint8_t sq_len = (uint8_t)(floor_log2_u64((idx_t)sq) + 1);
        if (bits_contained(k, k_len, sq, sq_len))
            return sq;
    }
    return 1;  /* 1 always appears */
}

/* Determine whether k satisfies the definition. */
static bool included(uint64_t k)
{
    uint64_t sq_div  = largest_square_div(k);
    uint64_t sq_sub  = largest_square_in_binary(k);
    return sq_div != sq_sub;
}

/* Ensure the cache can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (term_cap >= need)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety; sequence is defined for n >= 1 */

    /* Generate terms until we have at least n terms. */
    while (term_len < (size_t)n) {
        ++last_checked;
        if (included(last_checked)) {
            ensure_capacity(term_len + 1);
            term_cache[term_len++] = (seq_t) last_checked;
        }
    }
    return term_cache[n - 1];
}

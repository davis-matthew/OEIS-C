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
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   ilog2_idx, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/ilog2_idx.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* Test whether a binary pattern `val` of length `val_len` occurs
 * as a contiguous substring of the binary representation of `k`
 * of length `k_len`. */
static bool bits_contained(uint64_t k, uint8_t k_len,
                           uint64_t val, uint8_t val_len)
{
    if (val_len > k_len)
        return false;
    uint64_t mask;
    if (val_len == 64)
        mask = ~0ULL;
    else
        mask = ((uint64_t)1 << val_len) - 1;
    for (size_t shift = 0; shift <= (size_t)k_len - val_len; ++shift) {
        uint64_t sub = (k >> ((size_t)k_len - val_len - shift)) & mask;
        if (sub == val)
            return true;
    }
    return false;
}

/* Largest square dividing n (standard integer arithmetic). */
static uint64_t largest_square_div(uint64_t n)
{
    uint64_t rem = n;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        uint64_t square = p * p;
        while (rem % square == 0) {
            rem /= square;
        }
    }
    return n / rem;
}

/* Cache for previously found terms. */
static seq_t   *term_cache = NULL;
static size_t  term_cap   = 0;
static size_t  term_len   = 0;
static uint64_t last_checked = 0ULL;

/* Largest square whose binary representation occurs in that of k. */
static uint64_t largest_square_in_binary(uint64_t k)
{
    uint8_t k_len = (uint8_t)(ilog2_idx((idx_t)k) + 1);

    uint64_t max_val;
    if (k_len == 64)
        max_val = UINT64_MAX;
    else
        max_val = ((uint64_t)1 << k_len);

    uint64_t s_max = isqrt(max_val - 1ULL);

    for (uint64_t s = s_max; s > 0; --s) {
        uint64_t sq = s * s;
        uint8_t sq_len = (uint8_t)(ilog2_idx((idx_t)sq) + 1);
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
    term_cache = mem_resize(seq_t, term_cache, new_cap);
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

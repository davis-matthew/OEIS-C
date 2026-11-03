/*
 * A134733  Concatenation of next n imperfect numbers.
 * https://oeis.org/A134733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the next n imperfect numbers after
 *            skipping perfect numbers (6, 28, 496, 8128, 33550336,
 *            8589869056, 137438691328, 2305843008139952128). The (k‑th)
 *            imperfect numbers are the natural numbers that are not perfect.
 *            For each n the block consisting of the next n imperfect numbe...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 182 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Check if a given uint64_t is one of the known perfect numbers. */
static bool is_perfect_uint64(uint64_t x)
{
    /* Known perfect numbers (tested up to 2^64). */
    static const uint64_t perfects[] = {
        6ULL, 28ULL, 496ULL, 8128ULL,
        33550336ULL, 8589869056ULL, 137438691328ULL,
        2305843008139952128ULL
    };
    for (size_t i = 0; i < sizeof(perfects)/sizeof(perfects[0]); ++i) {
        if (x == perfects[i]) return true;
        if (x < perfects[i]) break;
    }
    return false;
}

/* Return the number of decimal digits of a value of type seq_t. */
static int digit_len_seq(seq_t v)
{
    if (v == 0) return 1;
    int len = 0;
    while (v > 0) {
        v /= 10;
        ++len;
    }
    return len;
}

/* General-purpose dynamic array growth for seq_t. */
static void grow_seq_array(seq_t **arr, size_t *cap)
{
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) {
        /* Out of memory – abort. */
        abort();
    }
    *arr = tmp;
    *cap = new_cap;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Compute triangular indices for block boundaries. */
    idx_t start_idx = (n - 1) * n / 2;        /* number of imperfect numbers before this block */
    idx_t end_idx   = start_idx + n;          /* exclusive end index */

    /* Static cache of produced imperfect numbers. */
    static seq_t *imperfect = NULL;
    static size_t imperfect_len = 0, imperfect_cap = 0;
    static uint64_t last_num = 0;             /* last numeric value examined */

    /* Grow the cache to hold at least end_idx imperfect numbers. */
    while (imperfect_len < (size_t)end_idx) {
        ++last_num;
        if (is_perfect_uint64(last_num))
            continue;                        /* skip perfect numbers */

        /* Append to cache. */
        if (imperfect_len == imperfect_cap)
            grow_seq_array(&imperfect, &imperfect_cap);
        imperfect[imperfect_len++] = (seq_t)last_num;
    }

    /* Concatenate the block [start_idx, end_idx). */
    seq_t result = 0;
    for (idx_t i = start_idx; i < end_idx; ++i) {
        seq_t val = imperfect[i];
        int digits = digit_len_seq(val);
        result = result * pow10((idx_t)digits) + val;
    }

    return result;
}

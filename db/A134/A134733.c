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
 * Helpers:   is_perfect_uint64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/is_perfect_uint64.h"

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
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
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

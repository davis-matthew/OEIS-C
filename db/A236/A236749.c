/*
 * A236749  Positive integers n such that n^2 divided by the digital sum of n
 *          is a cube.
 * https://oeis.org/A236749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 16, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_sum, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: list of positive integers n such that n^2 divided by the digital sum of n is a perfect cube. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_cube_root.h"

/* Persistent storage for the generated terms */
static seq_t      *seq = NULL;   /* array of found terms */
static size_t      seq_len = 0;  /* number of terms stored */
static size_t      seq_cap = 0;  /* capacity of seq */
static idx_t        last_checked = 1;  /* next integer to test */

/* Test whether a candidate satisfies the condition */
static bool is_good(idx_t cand) {
    seq_t n   = (seq_t)cand;               /* candidate as seq_t */
    seq_t s   = digit_sum(n);              /* decimal digit sum of n */

    /* n^2 must be divisible by its digit sum */
    seq_t n2  = n * n;
    if (n2 % s != 0) return false;

    /* value to test for being a perfect cube */
    seq_t val = n2 / s;
    seq_t r   = floor_cube_root(val);
    return (r * r * r == val);
}

/* Sequence accessor required by the build system */
seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for indices < 1 */

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        if (is_good(last_checked)) {
            /* Grow the array if necessary */
            if (seq_len >= seq_cap) {
                size_t new_cap = seq_cap ? (seq_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                seq  = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
    return seq[n - 1];
}

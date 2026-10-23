/*
 * A115163  Numbers that are not the sum of two triangular numbers and a
 *          nonnegative cube.
 * https://oeis.org/A115163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are NOT the sum of two triangular numbers and a non‑negative cube.
   The function searches for the next integer that has no representation
   x = T_i + T_j + c^3 with T_k = k(k+1)/2, c >= 0. */
/*  The first terms are 41, 104, 188, 923, ... (OEIS A115163). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

static seq_t *a_cache = NULL;    /* a_cache[0] = a(1) */
static size_t  a_len   = 0;      /* number of cached terms */

/* Helper: test whether x is a perfect square. */
static bool is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Helper: test whether a given value can be expressed as a sum of two
   triangular numbers and a non‑negative cube. */
static bool representable(seq_t val)
{
    for (idx_t c = 0;; ++c) {
        seq_t cube = (seq_t)c * c * c;
        if (cube > val) break;
        seq_t y = val - cube;

        /* Iterate triangular numbers T_r = r(r+1)/2 */
        seq_t r   = 0;
        seq_t t1  = 0;  /* T_0 = 0 */
        while (t1 <= y) {
            seq_t rem = y - t1;
            /* rem must also be triangular: 8*rem+1 should be a perfect square */
            if (is_square(rem * 8 + 1)) return true;

            ++r;
            t1 = r * (r + 1) / 2;
        }
    }
    return false;
}

/* Ensure the cache has at least n terms.  We generate terms on demand
   and store them in a_cache.  Calls may arrive in any order. */
static void ensure_terms(idx_t n)
{
    if (a_len >= n) return;

    seq_t val = (a_len == 0) ? 1 : a_cache[a_len - 1] + 1;

    while (a_len < n) {
        /* grow cache if necessary */
        if (a_cache == NULL) {
            a_cache = (seq_t *)malloc(4 * sizeof(seq_t));
            if (!a_cache) abort();
            a_len = 0;
        } else if (a_len == 4) {
            seq_t *tmp = (seq_t *)realloc(a_cache, 8 * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
        }

        if (!representable(val)) {
            a_cache[a_len++] = val;
        }
        ++val;
    }
}

/* Main sequence function with external linkage. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; OEIS offset starts at 1 */

    ensure_terms(n);
    return a_cache[n - 1];
}

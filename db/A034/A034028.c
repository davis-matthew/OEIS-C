/*
 * A034028  Numbers n not of form (x^2+2y^2 with x >= y >= 0).
 * https://oeis.org/A034028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A034028 – numbers n not of the form x^2 + 2y^2 (x ≥ y ≥ 0)
   Implementation: brute‑force search with memoisation.
   For each integer m ≥ 2 we test whether it can be written as x^2 + 2y^2
   with x ≥ y.  If not, m is stored; the n‑th stored value is a(n).
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

/* static storage for the list of excluded (non‑representable) numbers */
static seq_t *slice   = NULL;   /* array of values */
static size_t slice_len = 0;    /* number of stored values */
static size_t slice_cap = 0;    /* allocated capacity of slice */
static seq_t next_test = 0;     /* next integer to test for representability */

/* ensure the slice can hold at least new_cap elements */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= slice_cap) return;
    size_t cap = slice_cap ? slice_cap * 2 : 8;
    while (cap < new_cap) cap <<= 1;
    seq_t *p = (seq_t *)realloc(slice, cap * sizeof(seq_t));
    if (!p) abort();   /* out of memory – safe for the build system */
    slice    = p;
    slice_cap = cap;
}

/* test whether m can be written as x^2 + 2y^2 with x ≥ y ≥ 0 */
static bool is_representable(seq_t m)
{
    if (m < 2) return false;
    for (seq_t y = 0; ; ++y) {
        seq_t y2    = y * y;             /* y² */
        seq_t twice = y2 + y2;           /* 2·y² */
        if (twice > m) break;
        seq_t rem = m - twice;          /* remaining to be a perfect square */
        seq_t x = floor_sqrt(rem);
        if (x * x == rem && x >= y) return true;  /* found valid representation */
    }
    return false;
}

/* fill slice until it contains at least n elements */
static void ensure_n(idx_t n)
{
    while (slice_len < n) {
        if (next_test < 2) next_test = 2;
        if (!is_representable(next_test)) {
            ensure_capacity(slice_len + 1);
            slice[slice_len++] = next_test;
        }
        ++next_test;
    }
}

/* public interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* OEIS offset 1; 0 is a safe value for n < 1 */
    ensure_n(n);
    return slice[n - 1];
}

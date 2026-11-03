/*
 * A118490  Squares for which both the sum of the digits and the product of
 *          the digits is a triangular number.
 * https://oeis.org/A118490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = k² where k² is the nth square such that * the sum of its
 *            decimal digits and the product of its digits * are both
 *            triangular numbers. * The function searches squares in
 *            increasing order, storing * found terms in a static cache so
 *            that arbitrary (non‑monotonic) * calls to a(idx_t n) are answ...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   digit_product, digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Test whether x is a triangular number: x = m(m+1)/2 for some m ≥ 0.
   A number is triangular iff 8*x + 1 is a perfect square. */
static bool is_triangular(seq_t x)
{
    if (x == 0) return true;
    seq_t y = (seq_t)8 * x + 1;
    seq_t s = floor_sqrt(y);
    return s * s == y;
}

seq_t a(idx_t n)
{
    static idx_t        curr_k = 0;        /* integer whose square has just been checked */
    static seq_t       *terms  = NULL;     /* cache of qualifying squares */
    static size_t       terms_len = 0;      /* number of terms stored */
    static size_t       terms_cap = 0;      /* capacity of cache */

    /* Extend the cache until we have at least n terms. */
    while (terms_len < n) {
        seq_t sq = (seq_t)curr_k * (seq_t)curr_k;   /* current square */

        if (is_triangular(digit_sum(sq)) &&
            is_triangular(digit_product(sq))) {
            /* Grow the cache if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) /* handle allocation failure gracefully */ {
                    abort();
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = sq;
        }
        /* Move to the next integer. */
        curr_k++;
    }
    return terms[n - 1];
}

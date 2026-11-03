/*
 * A040761  Continued fraction for sqrt(790).
 * https://oeis.org/A040761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(790) computed by the standard
 *            algorithm for quadratic irrationals
 * Range:     u8 none, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 91 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cf = NULL;
    static size_t cf_cap = 0;
    static size_t cf_len = 0;      /* number of terms stored in cf */

    /* state for generating subsequent terms */
    static seq_t m = 0;            /* multiplier term */
    static seq_t d = 1;            /* divisor term */
    static seq_t prev_a = 0;       /* a(k‑1) */
    static seq_t a_start = 0;      /* floor(sqrt(790)) */
    static const seq_t D = (seq_t)790;

    /* initialize cache and state on first call */
    if (cf == NULL) {
        a_start = (seq_t)floor_sqrt(D);
        prev_a = a_start;

        cf_cap = 16;
        cf = mem_new(seq_t, cf_cap);
        if (cf == NULL) return 0;          /* allocation failure – unlikely */
        cf[0] = a_start;
        cf_len = 1;
    }

    /* compute terms up to n if necessary */
    while (cf_len <= (size_t)n) {

        /* standard continued‑fraction recurrence */
        m = d * prev_a - m;
        d = (D - m * m) / d;
        seq_t next_a = (a_start + m) / d;

        /* grow cache if needed */
        if (cf_len == cf_cap) {
            size_t new_cap = cf_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = mem_resize(seq_t, cf, new_cap);
            if (tmp == NULL) return 0;    /* allocation failure – unlikely */
            cf = tmp;
            cf_cap = new_cap;
        }

        cf[cf_len++] = next_a;
        prev_a = next_a;
    }

    return cf[(size_t)n];
}

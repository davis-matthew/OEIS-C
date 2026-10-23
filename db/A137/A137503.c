/*
 * A137503  Number of Frobenius equivalence classes of size n over GF(2^n)
 *          with their trace equal to the trace of their inverse.
 * https://oeis.org/A137503
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   int_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (2*w(n))/n
 * Recurrence and definitions (OEIS A137503):
 *   b(1) = 0,          b(2) = 1
 *   b(n) = 2^(n-1) - b(n-1) - 2*b(n-2) - 3        (n ≥ 3)
 *
 *   c(1) = 1
 *   c(n) = 2^(n-1) - Σ_{d|n , d<n} c(d)           (n ≥ 2)
 *
 *   w(n) = b(n) 
 *          - Σ_{d|n , 1<d<n , d even}   c(n/d)
 *          - Σ_{d|n , 1<d<n , d odd}    w(n/d)
 *
 *   a(n) = 2*w(n) / n
 *
 * The implementation pre‑computes b, c, w up to the requested index and
 * memoises the results for subsequent calls.  It uses the utility
 * functions pow2(idx_t) and int_sqrt(idx_t) that are supplied by the
 * build environment.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */
#include "../utils/int_sqrt.h"      /* idx_t int_sqrt(idx_t n); */

static seq_t *b = NULL;          /* b[1..]  */
static seq_t *c = NULL;          /* c[1..]  */
static seq_t *w = NULL;          /* w[1..]  */
static size_t current_size = 0;   /* number of allocated slots (≥ max index +1) */

/* Expand the arrays so that indices up to n are usable. */
static void ensure_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (current_size >= needed) return;

    seq_t *new_b = (seq_t *)realloc(b, needed * sizeof(seq_t));
    seq_t *new_c = (seq_t *)realloc(c, needed * sizeof(seq_t));
    seq_t *new_w = (seq_t *)realloc(w, needed * sizeof(seq_t));
    if (!new_b || !new_c || !new_w) {
        /* Allocation failure: let the caller handle it. */
        return;
    }

    /* Initialise the new slots to zero. */
    for (size_t i = current_size; i < needed; ++i) {
        new_b[i] = 0;
        new_c[i] = 0;
        new_w[i] = 0;
    }

    b = new_b;
    c = new_c;
    w = new_w;
    current_size = needed;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined only for n ≥ 2 */

    ensure_size(n);

    /* Initialise fixed values if not already set. */
    if (b[1] == 0) {
        b[1] = 0;
        c[1] = 1;
        w[1] = 0;                /* w(1) is not used in the formula for n ≥ 2 */
    }
    if (n >= 2 && b[2] == 0) {
        b[2] = 1;
    }

    /* Compute sequences up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* ----- b[i] ----- */
        if (i >= 3) {
            seq_t powv = pow2(i - 1);
            seq_t tmp = b[i - 1];
            tmp += b[i - 2] << 1;   /* 2 * b[i-2] */
            tmp += 3;
            b[i] = powv - tmp;
        }

        /* ----- c[i] ----- */
        seq_t sum_c = 0;
        idx_t root = int_sqrt(i);
        for (idx_t d = 1; d <= root; ++d) {
            if (i % d == 0) {
                if (d < i) sum_c += c[d];
                idx_t nd = i / d;
                if (nd < i && nd != d) sum_c += c[nd];
            }
        }
        c[i] = pow2(i - 1) - sum_c;

        /* ----- w[i] ----- */
        seq_t sum_even_c = 0;
        seq_t sum_odd_w  = 0;
        for (idx_t d = 1; d <= root; ++d) {
            if (i % d == 0) {
                /* divisor d */
                if (d > 1 && d < i) {
                    idx_t nd = i / d;
                    if (d & 1) sum_odd_w += w[nd];
                    else      sum_even_c += c[nd];
                }
                /* divisor nd = i/d */
                idx_t nd = i / d;
                if (nd != d && nd > 1 && nd < i) {
                    idx_t d2 = d;          /* the paired divisor */
                    if (nd & 1) sum_odd_w += w[d2];
                    else      sum_even_c += c[d2];
                }
            }
        }
        w[i] = b[i] - sum_even_c - sum_odd_w;
    }

    /* Return a(n) = 2*w(n)/n */
    seq_t result = w[n];
    result <<= 1;                   /* multiply by 2 */
    return result / (seq_t)n;
}

/*
 * A220180  E.g.f.: exp( Sum_{n>=1} (n+1)^(n-1) * x^n / n ).
 * https://oeis.org/A220180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n−1)!/(n−k)! * (k+1)^(k−1) * a(n−k) for n>0
 *            with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow_int.h"

/* Static cache for computed a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;

/* Static cache for f(k) = (k+1)^(k−1) */
static seq_t *f_cache = NULL;
static size_t f_cap = 0;

/* Return cached or compute f(k) = (k+1)^(k−1).  k >= 1. */
static seq_t get_f(idx_t k)
{
    if (k < f_cap) return f_cache[k];

    size_t new_cap = f_cap ? f_cap : 1;
    while (new_cap <= (size_t)k) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                     /* allocation failure – unlikely */
    f_cache = tmp;

    /* Fill new entries.  f_cache[0] is unused; set to 0 to avoid bogus pow. */
    for (size_t i = f_cap; i < new_cap; ++i) {
        if (i == 0) {
            f_cache[0] = 0;
            continue;
        }
        f_cache[i] = pow_int((seq_t)(i + 1), (idx_t)(i - 1));
    }
    f_cap = new_cap;
    return f_cache[k];
}

/* Ensure a_cache has capacity for at least n+1 elements and compute new values */
static void ensure_cache(idx_t n)
{
    if (n < a_cap) return;                      /* already available */

    size_t new_cap = a_cap ? a_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure – unlikely */
    a_cache = tmp;

    for (size_t i = a_cap; i < new_cap; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = 0;
        seq_t coeff = 1;
        for (idx_t k = 1; k <= (idx_t)i; ++k) {
            seq_t fk   = get_f(k);
            /* term = coeff * fk * a[i-k] */
            sum += coeff * fk * a_cache[i - k];
            coeff *= (seq_t)(i - k);          /* next coefficient factor */
        }
        a_cache[i] = sum;
    }
    a_cap = new_cap;
}

/* Public interface ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}

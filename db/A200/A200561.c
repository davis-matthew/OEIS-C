/*
 * A200561  Expansion of -2*x / ( (2*x-1)*(4*x^2+3*x+1) ).
 * https://oeis.org/A200561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) + 8*a(n-3) Initial values: a(1)=2,
 *            a(2)=-2, a(3)=6 (Recurrence holds for all n ≥ 4)
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 31, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* array of computed values, 1‑based stored as 0‑based */
static size_t cachecap = 0;      /* allocated capacity of cache */
static size_t max_computed = 0;  /* number of values already filled (≥ 0) */

/* Ensure cache can hold at least 'n' elements (1‑based). */
static void ensure_cache(idx_t n) {
    size_t need = (size_t)n;              /* truncates if n > SIZE_MAX, but n is small here */
    if (need <= cachecap) return;         /* already enough space */

    size_t newcap = cachecap > 0 ? cachecap : 16;
    while (newcap < need) newcap <<= 1;   /* double until sufficient */

    cache = seq_realloc(cache, cachecap, newcap);  /* seq_realloc is part of the utils set */
    cachecap = newcap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;            /* undefined for n<1 */

    ensure_cache(n);

    /* If already computed, just return. */
    if ((size_t)n <= max_computed) {
        return cache[(size_t)n - 1];
    }

    /* Compute missing terms up to n. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
        } else if (i == 2) {
            cache[1] = (seq_t)-2;
        } else if (i == 3) {
            cache[2] = (seq_t)6;
        } else {
            seq_t term = -cache[i - 2]          /* -a(i-1) */
                       + (seq_t)2 * cache[i - 3]/* +2*a(i-2) */
                       + (seq_t)8 * cache[i - 4];/* +8*a(i-3) */
            cache[i - 1] = term;
        }
    }
    max_computed = (size_t)n;
    return cache[(size_t)n - 1];
}

/*
 * A137419  a(n) = (a(n - 4) + 1 - ((-1)^a(a(n - 1)) + 1)*(a(a(n - 1)) - a(a(n
 *          - 2)))/2).
 * https://oeis.org/A137419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = a(n-4) + 1 - ((-1)^a(a(n-1)) + 1)*(a(a(n-1)) -
 *            a(a(n-2)))/2 Initial values: a(1)=1, a(2)=2, a(3)=2, a(4)=1
 * Range:     i8 n <= 132, i16 n <= 132, i32 n <= 132, i64 n <= 132,
 *            i128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoisation cache for a(n) */
static seq_t *cache = NULL;          /* cache[0] holds a(1), cache[1] holds a(2), … */
static size_t cache_size = 0;        /* number of elements stored in cache */

/* Computes sequence A137419 term a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n ≥ 1 in official use; guard anyway */

    /* initialise cache with first four terms if not already done */
    if (cache_size < 4) {
        cache_size = 4;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)2;  /* a(2) */
        cache[2] = (seq_t)2;  /* a(3) */
        cache[3] = (seq_t)1;  /* a(4) */
    }

    /* if the requested term is already computed */
    if ((size_t)n <= cache_size) {
        return cache[(size_t)n - 1];
    }

    /* extend cache up to n and compute new terms */
    size_t new_sz = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) return 0; /* unrecoverable allocation failure; unlikely in tests */
    cache = tmp;

    for (size_t i = cache_size + 1; i <= new_sz; ++i) {
        /* indices for earlier terms (1‑based) */
        seq_t an1 = cache[i - 2];          /* a(i-1) */
        seq_t an2 = cache[i - 3];          /* a(i-2) */
        seq_t an4 = cache[i - 5];          /* a(i-4) */

        /* inner indices for nested calls: must be already computed */
        idx_t inner1 = (idx_t)an1;         /* a(a(i-1)) -> a(inner1) */
        idx_t inner2 = (idx_t)an2;         /* a(a(i-2)) -> a(inner2) */

        seq_t a_inner1 = cache[inner1 - 1];
        seq_t a_inner2 = cache[inner2 - 1];

        /* compute (-1)^{a_inner1} as 1 or -1 */
        seq_t sign = ((a_inner1 & (seq_t)1) == 0) ? (seq_t)1 : (seq_t)(-1);

        seq_t termDiff = a_inner1 - a_inner2;
        seq_t factor = ((sign + (seq_t)1) * termDiff) / (seq_t)2;

        seq_t value = an4 + (seq_t)1 - factor;
        cache[i - 1] = value;
    }

    cache_size = new_sz;
    return cache[(size_t)n - 1];
}

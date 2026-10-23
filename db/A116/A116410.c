/*
 * A116410  Expansion of (1-x-2x^2+sqrt(1-2x-3x^2))/(2*(1-2x-3x^2)).
 * https://oeis.org/A116410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A116410
 * a(n) = ((3^n + 2*0^n)/3 + Σ_{k=0..⌊n/2⌋} C(n,2k)·C(2k,k)) / 2
 *        (with 0^0 = 1, so 2*0^n = 2 when n = 0, else 0)
 * The function memoises previously computed values for O(1) random access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/pow3.h"         /* seq_t pow3(idx_t n) */
#include "../utils/binomial.h"     /* seq_t binomial(idx_t n, idx_t k) */

/* Memoisation cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute one term of the sequence using the explicit formula */
static seq_t compute_term(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = binomial(n, 2 * k);
        term = term * binomial(2 * k, k);
        sum = sum + term;
    }

    seq_t pow3n = pow3(n);
    seq_t part  = pow3n;
    if (n == 0) {
        part = part + (seq_t)2;
    }
    part = part / (seq_t)3;

    seq_t tot   = sum + part;
    seq_t result = tot / (seq_t)2;
    return result;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = compute_term((idx_t)0);
    }

    /* Extend cache so that it contains a[n] */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_ptr == NULL) abort();
        cache = new_ptr;
        for (size_t i = cache_cap; i < new_cap; ++i) {
            cache[i] = compute_term((idx_t)i);
        }
        cache_cap = new_cap;
    }

    return cache[n];
}

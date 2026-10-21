/*
 * A080449  a(1) = 5, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1). Also a(n) is not
 *          divisible by 10.
 * https://oeis.org/A080449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   mul_overflows_u64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A080449.
 *
 *  a(1) = 5
 *  a(n) = smallest non‑trivial multiple of a(n‑1) that
 *         has exactly n decimal digits,
 *         is not 10·a(n‑1), and
 *         is not divisible by 10.
 *
 * The routine uses linear search for the multiplier
 * (which is always near 10) and memoises all terms up
 * to the largest index requested.  The implementation
 * works for any seq_t supplied by the build
 * (uint8_t … uint64_t, unsigned __int128 or an
 * arbitrary‑precision class).  For 64‑bit builds it
 * guards against integer overflow by using
 * unsigned __int128 for intermediate products.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"          /* pow10(idx_t) -> seq_t */
#include "../utils/mul_overflows_u64.h"

static seq_t *cache = NULL;        /* 1‑based: cache[1] = a(1) */
static size_t cache_cap = 0;       /* current allocated size (in seq_t) */
static idx_t computed = 0;         /* highest index computed so far */

/* Ensure that the cache array has space for indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);           /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute one term from its predecessor.  Returns the value of a(n). */
static seq_t next_term(const seq_t prev, idx_t n)
{
    seq_t lower = pow10((idx_t)n - 1);   /* 10^(n-1) */
    seq_t upper = pow10((idx_t)n);       /* 10^n      */

    /* k must be at least ceil(lower / prev).  Avoid division by zero. */
    seq_t start_k = (lower + prev - (seq_t)1) / prev;
    if (start_k < (seq_t)2) start_k = (seq_t)2;

    for (seq_t k = start_k; ; ++k) {
        if (k == (seq_t)10) continue;   /* forbidden multiplier */

        seq_t prod;
        if (sizeof(seq_t) == sizeof(uint64_t)) {           /* 64‑bit build */
            uint64_t prod64;
            if (mul_overflows_u64((uint64_t)prev, (uint64_t)k, &prod64))
                continue;                     /* product too large */
            prod = (seq_t)prod64;
        } else {
            prod = prev * k;                    /* arbitrary precision */
        }

        if ( (prod % (seq_t)10U) == (seq_t)0 ) continue;  /* divisible by 10 */

        if (prod >= lower && prod < upper) return prod;
    }
}

/* Public sequence function.  Accepts any n >= 1 in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, return 0 */

    /* Compute as many terms as required. */
    if (n > computed) {
        ensure_cache_capacity(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == (idx_t)1)
                cache[i] = (seq_t)5;
            else
                cache[i] = next_term(cache[i - 1], i);
        }
        computed = n;
    }
    return cache[n];
}

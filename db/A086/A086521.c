/*
 * A086521  Number of tandem duplication trees on n duplicated gene segments.
 * https://oeis.org/A086521
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n)/2 for n ≥ 3 b(1) = b(2) = 1 b(n) =
 *            Σ_{k=1}^{⌊(n+1)/3⌋} (-1)^{k+1} * binom(n+1-2k, k) * b(n-k) (n ≥
 *            3) a(2) = 1 (exception, as b(2) is odd)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for required utilities and standard types */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"          /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/ensure_capacity.h" /* void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size); */

/* Static cache for b(n) values; indices 0.. */
static seq_t *b_cache = NULL;   /* pointer to cached values; b_cache[0] unused */
static size_t b_size = 0;       /* first unused index (i.e., size of computed part) */
static size_t b_cap = 0;        /* current allocated capacity (size_t) */

/* Ensure the cache is allocated, initialized, and large enough for index n (inclusive) */
static void ensure_b_cache(idx_t n)
{
    /* Allocate or enlarge the cache if necessary */
    if (b_cap <= (size_t)n) {
        size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
        ensure_capacity(needed, &b_cap, (void **)&b_cache, sizeof(seq_t));
    }

    /* Initialize base values only once */
    if (b_size == 0) {
        b_cache[0] = (seq_t)0;   /* unused placeholder */
        b_cache[1] = (seq_t)1;   /* b(1) */
        b_cache[2] = (seq_t)1;   /* b(2) */
        b_size   = 3;            /* next index to compute */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)           /* unspecified for n < 2 */
        return (seq_t)0;

    if (n == 2)          /* a(2) = 1 by definition */
        return (seq_t)1;

    /* Ensure cache ready for required index */
    ensure_b_cache(n);

    /* Compute b(i) for i = b_size .. n */
    for (idx_t i = b_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t lim = (i + 1) / 3;   /* floor((i+1)/3) */
        for (idx_t k = 1; k <= lim; ++k) {
            seq_t term = binom(i + 1 - 2 * k, k);
            term = term * b_cache[i - k];
            if (k & 1)          /* k odd → add */
                sum = sum + term;
            else                /* k even → subtract */
                sum = sum - term;   /* safe: term ≤ current sum in this recurrence */
        }
        b_cache[i] = sum;
    }
    b_size = n + 1;          /* cache now holds entries up to n */

    /* Return a(n) = b(n)/2 for n ≥ 3 */
    return b_cache[n] / (seq_t)2;
}

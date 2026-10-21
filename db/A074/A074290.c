/*
 * A074290  Difference between Kolakoski(n)=A000002(n) and 1 (n odd) or 2 (n
 *          even).
 * https://oeis.org/A074290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Kolakoski(n) - ( (n odd?1:2) )
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Persistent cache for Kolakoski sequence (1‑based, cache[0] unused) */
static seq_t *kolakoski_cache = NULL;
static size_t cache_cap = 0;     /* current allocated capacity (>= max_n) */
static idx_t max_n = 0;          /* largest n precomputed */

/* Current state of the generator */
static idx_t run_index = 2;      /* run number we are generating (initially 2) */
static idx_t cur = 0;            /* number of terms already written in current run */

/* Ensure the cache holds at least n terms of Kolakoski sequence */
static void grow_until(idx_t n)
{
    if (n <= max_n)
        return;

    /* Grow allocation if necessary (doubling strategy) */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(kolakoski_cache,
                                      (new_cap + 1) * sizeof(seq_t));
        if (!tmp)
            exit(1);                 /* allocation failed */
        kolakoski_cache = tmp;
        cache_cap = new_cap;
    }

    /* First call: initialise cache with first Kolakoski term */
    if (max_n == 0) {
        kolakoski_cache[1] = (seq_t)1;   /* first term is 1 */
        max_n = 1;
        run_index = 2;   /* next run to generate */
        cur = 0;
    }

    /* Generate terms until we reach n */
    while (max_n < n) {
        /* Determine value for the current run (odd runs -> 1, even runs -> 2) */
        seq_t val = (run_index & 1ULL) ? (seq_t)1 : (seq_t)2;
        kolakoski_cache[++max_n] = val;
        cur++;                                   /* one more term in current run */

        /* Check if current run is finished */
        if (cur == kolakoski_cache[run_index]) {
            run_index++;      /* move to next run */
            cur = 0;          /* reset counter for new run */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)               /* offset is 1, behaviour below irrelevant */
        return 0;

    grow_until(n);
    seq_t term = kolakoski_cache[n];
    seq_t subtract = (idx_t)(n & 1ULL) ? (seq_t)1 : (seq_t)2;
    return term - subtract;
}

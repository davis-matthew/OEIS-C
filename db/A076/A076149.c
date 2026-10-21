/*
 * A076149  Expansion of x^2(3+2x)/(1-x-5x^2-3x^3).
 * https://oeis.org/A076149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A076149
 * Recurrence:  a(1) = 0;  a(n+1) = 3*a(n) – (-1)^n * (n+2)
 * (implemented iteratively with memoisation)
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of computed terms */
static seq_t *cache = NULL;         /* array of a(i) values, index 1 … n */
static idx_t  cache_cap = 0;        /* allocated size (highest index)   */
static idx_t  max_computed = 0;     /* highest index that has been filled */

/* Ensure |cache| ≥ n+1, discarding any unfilled slots. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* realloc is required in C++ and safe in C.  The cast is needed
       for the C++ compiler.  We allocate n+1 elements: indices 0 … n;
       index 0 is unused. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) {
        free(cache);
        exit(EXIT_FAILURE);  /* out of memory */
    }
    cache      = tmp;
    cache_cap  = n;
    /* newly allocated entries are left uninitialised; they will be
       written when computed. */
}

/* Public routine implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) is undefined, return 0 safely */
        return (seq_t)0;

    /* First call: initialise base case */
    if (max_computed == 0) {
        ensure_cache(n);
        cache[1]      = (seq_t)0;  /* a(1) = 0 */
        max_computed  = 1;
    } else if (n > cache_cap) {    /* enlarge cache if needed */
        ensure_cache(n);
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t term  = 3 * prev;           /* 3*a(i-1) */
        if (i % 2 == 1) {                 /* i odd → subtract (i+1) */
            term -= (seq_t)(i + 1);
        } else {                          /* i even → add (i+1)  */
            term += (seq_t)(i + 1);
        }
        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}

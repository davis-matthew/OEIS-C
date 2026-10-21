/*
 * A215665  a(n) = 3*a(n-2) - a(n-3), with a(0)=0, a(1)=a(2)=-3.
 * https://oeis.org/A215665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-3) with a(0)=0, a(1)=a(2)=-3
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Forward declaration of the function to be implemented */
seq_t a(idx_t n);

/* Internal cache for memoisation */
static seq_t *seq_cache = NULL;
static idx_t seq_cap   = 0;  /* Allocated size of the cache   */
static idx_t seq_last  = 0;  /* Highest index that has been computed */

static void ensure_computed(idx_t n) {
    if (!seq_cache) {
        /* Allocate an initial buffer large enough for the requested index */
        size_t init_cap = n + 1;
        if (init_cap < 8) init_cap = 8;               /* protect small n */
        seq_cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)-3;
        seq_cache[2] = (seq_t)-3;
        seq_cap  = init_cap;
        seq_last = 2;
    }
    if (n <= seq_last) return;                       /* already computed */

    /* Grow the cache if necessary */
    if (n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;              /* realloc never fails in our use case      */
        seq_cap   = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_t term = (seq_t)3 * seq_cache[i - 2] - seq_cache[i - 3];
        seq_cache[i] = term;
    }
    seq_last = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)-3;
    ensure_computed(n);
    return seq_cache[n];
}

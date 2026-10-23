/*
 * A144537  a(0) = 3; for n > 0, a(n) = sqrt(3)*(P^(2*n)-M^(2*n))/8 +
 *          (3/4)*(P^n+M^n) + 1/2, where P = 7+4*sqrt(3), M = 7-4*sqrt(3).
 * https://oeis.org/A144537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=53, a(2)=8294, a(3)=1582697, a(4)=306670058,
   a(5)=59487327869; for n>5:
   a(n) = 209*a(n-1) - 2926*a(n-2) + 2926*a(n-3) - 209*a(n-4) + a(n-5) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* capacity of cache array */

static const seq_t init_vals[6] = {
    (seq_t)3,        /* a(0) */
    (seq_t)53,       /* a(1) */
    (seq_t)8294,     /* a(2) */
    (seq_t)1582697,  /* a(3) */
    (seq_t)306670058,/* a(4) */
    (seq_t)59487327869/* a(5) */
};

seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = ((size_t)1 << 6);          /* start with 64 cells */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) /* allocation failed – undefined behaviour */
            return (seq_t)0;
        cache[0] = init_vals[0];
        cache_len = 1;
    }

    /* enlarge storage if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;      /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* realloc failed – undefined behaviour */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 6) {
            cache[i] = init_vals[i];
        } else {
            seq_t term = (seq_t)209 * cache[i - 1]
                       + (seq_t)2926 * cache[i - 3]
                       +         cache[i - 5];
            term -= ( (seq_t)2926 * cache[i - 2]
                     + (seq_t)209 * cache[i - 4] );
            cache[i] = term;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

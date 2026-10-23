/*
 * A009940  a(n) = n!*L_{n}(1), where L_{n}(x) is the n-th Laguerre
 *          polynomial.
 * https://oeis.org/A009940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, for n>1: a(n)=2*(n-1)*a(n-1)-(n-1)^2*a(n-2)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/*  Static cache for computed values */
static seq_t *cache = NULL;           /* array of sequence values      */
static size_t cache_size = 0;         /* allocated size of cache       */
static idx_t  computed_up_to = 0;     /* highest index already computed */

/* Ensure cache can hold values up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_size) return;          /* already big enough */

    size_t new_cap = cache_size ? cache_size : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until >= n */

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                            /* allocation failed */
    cache = tmp;
    cache_size = new_cap;
}

/* Initialise the two known base values */
static void init_base(void)
{
    if (computed_up_to == 0) {
        cache[0] = (seq_t)1;                     /* a(0) = 1           */
        if (cache_size > 1) {                    /* cache can hold a(1) */
            cache[1] = (seq_t)0;                 /* a(1) = 0           */
            computed_up_to = 1;                  /* we have values 0..1 */
        } else {
            computed_up_to = 0;                  /* only a(0) is usable */
        }
    }
}

/* Compute a(n) (any order of calls is allowed) */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);
    init_base();

    if ((idx_t)n <= computed_up_to) {
        return cache[(size_t)n];
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[(size_t)(i - 1)];    /* a(i-1) */
        seq_t a_nm2 = cache[(size_t)(i - 2)];    /* a(i-2) */

        seq_t t1 = (seq_t)(i - 1) * a_nm1;       /* (i-1)*a(i-1) */
        t1 = t1 + t1;                            /* 2*(i-1)*a(i-1) */

        seq_t t2 = (seq_t)(i - 1) * (seq_t)(i - 1); /* (i-1)^2 */
        t2 = t2 * a_nm2;                          /* (i-1)^2*a(i-2) */

        cache[(size_t)i] = t1 - t2;              /* a(i) */
    }

    computed_up_to = n;
    return cache[(size_t)n];
}

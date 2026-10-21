/*
 * A240631  Number of n X 3 0..1 arrays with no element equal to exactly three
 *          horizontal or vertical neighbors, with new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A240631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(n) = 4*a(n-1) + 4*a(n-2) +
 *            2*a(n-3) + 7*a(n-4) + 6*a(n-5) - a(n-6) - a(n-7) - a(n-8) for n
 *            ≥ 9, with initial values a(1)=4, a(2)=25, a(3)=129, a(4)=637,
 *            a(5)=3147, a(6)=15590, a(7)=77269, a(8)=382933
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below memoises all computed terms so that
   calls in arbitrary order behave correctly and run fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* forward declaration of the requested function name */
seq_t a(idx_t n);

/* internal cache data */
static seq_t *cache   = NULL;  /* cache[0] unused, cache[i] = a(i) */
static size_t cache_cap = 0;   /* number of elements allocated (including index 0) */
static size_t max_computed = 0;/* biggest index that has a valid value in cache */

/* initialise the known base terms once */
static void init_base(void)
{
    /* cache must already be large enough for indices 0..8 */
    cache[0] = 0;              /* unused sentinel */
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)25;
    cache[3] = (seq_t)129;
    cache[4] = (seq_t)637;
    cache[5] = (seq_t)3147;
    cache[6] = (seq_t)15590;
    cache[7] = (seq_t)77269;
    cache[8] = (seq_t)382933;
    max_computed = 8;
}

/* ensure the cache is large enough to hold indices up to n (inclusive)   */
/* Always allocate at least space for indices 0..8, regardless of n.     */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;         /* indices 0 … n */
    if (needed < 9)
        needed = 9;                        /* space for the base terms       */

    if (needed <= cache_cap)
        return;                            /* already enough                      */

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (tmp == NULL)
        abort();                            /* insufficient memory – fatal */

    cache = tmp;
    cache_cap = needed;

    /* The first time we grow the cache we must initialise the base terms. */
    if (max_computed == 0)
        init_base();
}

/* compute all missing terms up to and including n */
static void compute_upto(idx_t n)
{
    if (n <= max_computed)
        return;            /* nothing to do */

    /* start from one past the last computed index */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i)
    {
        seq_t v = 4 * cache[i - 1] + 4 * cache[i - 2] + 2 * cache[i - 3]
                + 7 * cache[i - 4] + 6 * cache[i - 5]
                - cache[i - 6] - cache[i - 7] - cache[i - 8];
        cache[i] = v;
    }
    max_computed = (size_t)n;
}

/* public entry point requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* OEIS does not define a(0) */

    ensure_capacity(n);
    compute_upto(n);
    return cache[(size_t)n];
}

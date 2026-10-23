/*
 * A182894  Number of weighted lattice paths in L_n having no (1,0)-steps at
 *          level 0. The members of L_n are paths of weight n that start at
 *          (0,0) , end on the horizontal axis and whose steps are of the
 *          following four kinds: an (1,0)-step with weight 1, an (1,0)-step
 *          with weight 2, a (1,1)-step with weight 2...
 * https://oeis.org/A182894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence * * a(n) = ((4*n-3)*a(n-1) -(n-3)*a(n-2) +
 *            3*a(n-3) * -3*(5*n-14)*a(n-4) -6*(n-3)*a(n-5) -6*(n-4)*a(n-6) *
 *            +4*(n-6)*a(n-7)) / n * * Initial values: * a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=2, a(4)=2, a(5)=4, * a(6)=12, a(7)=24. * * This
 *            recurrence comes from the conjectured relation given...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* External, predeclared types.  Do NOT define them here.
 *   typedef ... idx_t;
 *   typedef ... seq_t;
 */

/* Static cache of computed terms.  All functions below use it. */
static seq_t *cache = NULL;     /* pointer to array of seq_t */
static size_t cache_cap = 0;    /* allocated capacity (number of elements) */
static size_t cache_size = 0;   /* number of terms that have been computed */

/* Initialise the cache with the first eight terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;                     /* already initialised */

    /* Allocate space for eight terms. */
    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    assert(cache != NULL);

    /* Base values. */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)2;
    cache[5] = (seq_t)4;
    cache[6] = (seq_t)12;
    cache[7] = (seq_t)24;

    cache_size = 8;
}

/* Ensure that the cache has room for indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;                 /* enough space */

    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;           /* double until sufficient */

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    assert(new_ptr != NULL);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute terms up to 'up_to' (inclusive).  Assumes up_to >= 8. */
static void compute_terms(idx_t up_to)
{
    for (idx_t i = cache_size; i <= up_to; ++i) {
        /* Individual terms for the recurrence. */
        seq_t t1 = (seq_t)(4*i - 3) * cache[i-1];
        seq_t t2 = (seq_t)(i - 3) * cache[i-2];
        seq_t t3 = (seq_t)3 * cache[i-3];
        seq_t t4 = (seq_t)(15*i - 42) * cache[i-4];   /* 3*(5n-14) */
        seq_t t5 = (seq_t)(6 * (i - 3)) * cache[i-5];
        seq_t t6 = (seq_t)(6 * (i - 4)) * cache[i-6];
        seq_t t7 = (seq_t)(4 * (i - 6)) * cache[i-7];

        /* Positive and negative contributions. */
        seq_t sum_pos = t1 + t3 + t7;
        seq_t sum_neg = t2 + t4 + t5 + t6;

        seq_t numer = sum_pos - sum_neg;          /* numerator is non‑negative */
        cache[i] = numer / (seq_t)i;              /* division is exact */
    }
    cache_size = up_to + 1;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    init_cache();                     /* ensure base terms exist */

    if (n < (idx_t)cache_size)
        return cache[n];              /* already known */

    ensure_capacity(n);
    compute_terms(n);
    return cache[n];
}

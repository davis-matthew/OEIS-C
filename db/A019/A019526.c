/*
 * A019526  Poincaré series [or Poincare series] for depths of roots in a
 *          certain root system.
 * https://oeis.org/A019526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-5); initial values: a(1)=4, a(2)=5,
 *            a(3)=8, a(4)=13, a(5)=24. The recurrence holds for n≥6
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of sequence values   */
static size_t capacity = 0;      /* number of elements allocated      */
static idx_t computed = 0;       /* highest index that has been filled (1‑based) */

/* Ensure the cache can hold at least n elements. */
static void ensure_capacity(idx_t n)
{
    if (capacity < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation failure */
        cache = tmp;
        capacity = new_cap;
    }
}

/* Compute all terms up to and including index n. */
static void compute_until(idx_t n)
{
    if (computed >= n) return;    /* nothing to do */

    if (computed == 0) {
        /* initialise the first five terms */
        ensure_capacity(5);
        cache[0] = 4;
        cache[1] = 5;
        cache[2] = 8;
        cache[3] = 13;
        cache[4] = 24;
        computed = 5;
    }

    ensure_capacity(n);           /* make sure there is room for n */

    /* apply the recurrence from max(computed+1,6) */
    idx_t start = computed + 1;
    if (start < 6) start = 6;
    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - a(i-5) */
        cache[i-1] = (seq_t)2 * cache[i-2] - cache[i-6];
    }

    computed = n;
}

/* External sequence function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; callers ignore this case */
    compute_until(n);
    return cache[n-1];            /* cache is 0‑based, a(1) stored at cache[0] */
}

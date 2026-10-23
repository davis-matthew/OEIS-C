/*
 * A185459  Number of (n+1) X 2 binary arrays with no 2 X 2 subblock
 *          determinant equal to any horizontal or vertical neighbor 2 X 2
 *          subblock determinant.
 * https://oeis.org/A185459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 2*a(n-4) (n > 5) initial
 *            values: a(1)=16, a(2)=34, a(3)=92, a(4)=218, a(5)=540
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *a_cache = NULL;        /* cached terms, 0‑unused; entries 1..max */
static idx_t  a_cache_size = 0;      /* allocated size (≥ max+1)               */
static idx_t  a_max_computed = 0;    /* largest n for which a_cache[n] is valid */

static const seq_t init_vals[6] = {
    0,          /* dummy for index 0 */
    (seq_t)16,  /* a(1) */
    (seq_t)34,  /* a(2) */
    (seq_t)92,  /* a(3) */
    (seq_t)218, /* a(4) */
    (seq_t)540  /* a(5) */
};

/* ensure a_cache can index up to [0..needed-1] */
static void ensure_cache(idx_t needed)
{
    if (a_cache_size >= needed)
        return;

    /* grow to at least needed, doubling each step for amortised O(n) */
    idx_t new_size = a_cache_size ? a_cache_size * 2 : 4;
    while (new_size < needed)
        new_size *= 2;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) { /* allocation failure – abort with zero (undefined) */
        exit(1);
    }
    a_cache = tmp;
    a_cache_size = new_size;

    /* initialise newly allocated entries that are part of the fixed base */
    if (needed <= 6) {
        for (idx_t i = 0; i < needed; ++i)
            a_cache[i] = init_vals[i];
    } else if (a_cache_size >= 6) {
        for (idx_t i = 0; i < 6; ++i)
            a_cache[i] = init_vals[i];
    }

    /* ensure a_max_computed reflects the highest computed index */
    if (a_max_computed + 1 < 5)
        a_max_computed = 5;
}

/* sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for non‑positive indices */

    if (n <= 5)
        return init_vals[n];     /* fixed initial part */

    /* make sure cache is large enough */
    ensure_cache(n + 1);          /* indices 0..n inclusive */

    /* compute missing terms up to n */
    if (a_max_computed < 5)
        a_max_computed = 5;
    for (idx_t i = a_max_computed + 1; i <= n; ++i) {
        a_cache[i] = a_cache[i-1] +
                     (seq_t)2 * a_cache[i-2] +
                     (seq_t)3 * a_cache[i-3] +
                     (seq_t)2 * a_cache[i-4];
    }
    a_max_computed = n;
    return a_cache[n];
}

/*
 * A087836  a(n) = a(a(a(a(n-1)))) + a(n - a(n-1)) with a(1)=a(2)=1.
 * https://oeis.org/A087836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(a(a(n-1)))) + a(n - a(n-1)), a(1) = 1, a(2) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t   last_computed = 0;   /* highest index already computed   */
static idx_t   capacity      = 0;   /* allocated array capacity         */
static seq_t * cache           = NULL;/* one‑based index: cache[1] = a(1) */

/* ensure the internal cache can hold indices up to `n`. */
static void
ensure_cache(idx_t n)
{
    if (capacity < n) {
        idx_t new_cap = n + 16;                 /* give a little slack */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);                      /* out of memory */
        cache   = tmp;
        capacity = new_cap;
    }
}

seq_t
a(idx_t n)
{
    if (n <= 2)                                         /* a(1) = a(2) = 1 */
        return (seq_t)1;

    ensure_cache(n);

    /* initialise base cases if needed */
    if (last_computed < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last_computed = 2;
    }

    /* compute any missing terms up to `n` */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[(idx_t)a1];
        seq_t a3 = cache[(idx_t)a2];
        seq_t a4 = cache[(idx_t)a3];                  /* a(a(a(a(i-1)))) */

        seq_t val1 = a4;
        seq_t val2 = cache[i - (idx_t)a1];            /* a(i - a(i-1)) */

        cache[i] = val1 + val2;
    }

    last_computed = n;
    return cache[n];
}

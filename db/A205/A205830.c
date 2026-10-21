/*
 * A205830  Number of (n+1) X 3 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two.
 * https://oeis.org/A205830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 8*a(n-2) + 6*a(n-3) for n>4 Initial values:
 *            a(1)=111, a(2)=630, a(3)=3642, a(4)=21126
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed values */
static idx_t  last_n = 0;            /* largest n already computed  */
static size_t cap    = 0;            /* allocated capacity (elements) */

/* initial fixed terms, stored modulo seq_t */
static const seq_t init_vals[5] = {
    (seq_t)0,      /* unused index 0 */
    (seq_t)111,
    (seq_t)630,
    (seq_t)3642,
    (seq_t)21126
};

/* Ensure that cache has room for indices up to n (inclusive)   */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;                /* indices 0..n */
    if (need <= cap) return;

    /* grow capacity to at least need, double each time */
    size_t new_cap = (cap < 8 ? 8 : cap);
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – give up */
        /* In a production build you might handle it differently */
        exit(1);
    }
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n<1; return 0 as guard */

    ensure_capacity(n);

    if (last_n < n) {              /* compute forward up to n */
        for (idx_t i = last_n + 1; i <= n; ++i) {
            if (i <= 4)
                cache[i] = init_vals[i];
            else
                cache[i] = (seq_t)7 * cache[i-1]
                          - (seq_t)8 * cache[i-2]
                          + (seq_t)6 * cache[i-3];
        }
        last_n = n;
    }
    return cache[n];
}

/*
 * A204368  Number of (n+2) X 8 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) for n > 9 Initial values:
 *            a(1)=6121, a(2)=4355, a(3)=2574, a(4)=2956, a(5)=3280,
 *            a(6)=4856, a(7)=9272, a(8)=19880, a(9)=50056
 * Range:     u8 none, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* computed values, index 0 unused */
static idx_t cache_capacity = 0; /* allocated size (including index 0) */
static idx_t cache_last = 0;     /* largest n that has been computed */

/* initial 9 values, index 0 unused */
static const seq_t init_vals[10] = {
    0,
    6121, 4355, 2574, 2956, 3280, 4856, 9272, 19880, 50056
};

static void ensure_capacity(idx_t needed)
{
    if (cache_capacity >= needed) return;
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
    if (!tmp) return;                       /* allocation failed – keep old cache */
    cache = tmp;
    cache_capacity = needed;
}

static void init_terms(void)
{
    /* initialise the first nine terms once */
    if (cache_last != 0) return;
    if (cache_capacity < 10) return;        /* safety: cannot store them */
    for (idx_t i = 1; i <= 9; ++i) cache[i] = init_vals[i];
    cache_last = 9;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* behaviour for n<1 irrelevant */

    /* need storage up to n (index 0..n) */
    ensure_capacity(n + 1);
    if (!cache) return 0;                  /* allocation failed */

    init_terms();

    if (n <= cache_last) return cache[n];

    /* compute missing terms */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = 3 * cache[i-1] - 6 * cache[i-4] + 4 * cache[i-6];
        }
    }
    cache_last = n;
    return cache[n];
}

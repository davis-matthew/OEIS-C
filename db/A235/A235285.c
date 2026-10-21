/*
 * A235285  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 15*a(n-3) + 4*a(n-4) * + 18*a(n-5)
 *            - 12*a(n-6); * Initial values: a(1)=136, a(2)=188, a(3)=248,
 *            a(4)=380, * a(5)=544, a(6)=908
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* a[0 .. n]  (a[0] unused) */
static size_t cache_size = 0;      /* allocated bytes / sizeof(seq_t) */
static idx_t  max_cached = 0;     /* highest index that has been computed */

static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);          /* need indices 0..n */
    if (needed <= cache_size) return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp && needed) abort();              /* allocation failed */
    cache = tmp;
    cache_size = needed;
}

/* Initialise the first six terms (stored in indices 1..6) */
static void
fill_bases(void)
{
    if (max_cached >= 6) return;          /* bases already set */

    ensure_cache(6);
    static const seq_t base[7] = {
        0,      /* a[0] unused */
        136,    /* a[1] */
        188,    /* a[2] */
        248,    /* a[3] */
        380,    /* a[4] */
        544,    /* a[5] */
        908     /* a[6] */
    };
    for (idx_t i = 0; i <= 6; ++i) cache[i] = base[i];
    if (max_cached < 6) max_cached = 6;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* sequence is defined for n >= 1 */

    ensure_cache(n);
    fill_bases();

    if (n <= max_cached) return cache[n];

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i - 1]
                  + (seq_t)3 * cache[i - 2]
                  - (seq_t)15 * cache[i - 3]
                  + (seq_t)4 * cache[i - 4]
                  + (seq_t)18 * cache[i - 5]
                  - (seq_t)12 * cache[i - 6];
        cache[i] = val;
    }
    max_cached = n;
    return cache[n];
}

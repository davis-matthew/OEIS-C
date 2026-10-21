/*
 * A235080  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 10*a(n-3) - 5*a(n-4) Initial
 *            values: a(1)=38, a(2)=90, a(3)=202, a(4)=478
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Dynamic cache for memoisation */
static seq_t *cache = NULL;          /* allocated entries 0 … cache_cap-1 */
static size_t  cache_cap = 0;       /* number of allocated slots (size of cache) */
static idx_t   last_filled = 0;     /* largest index for which cache[index] is valid */

/* Ensure that cache has at least n+1 entries (indices 0 … n) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (cache == NULL) abort();
        cache_cap = n + 1;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise known terms (once) */
static void init_base(void)
{
    if (last_filled != 0) return;          /* already initialised */

    ensure_cache(4);                       /* need indices 0 … 4 */

    cache[0] = (seq_t)0;                   /* unused but defined */
    cache[1] = (seq_t)38;
    cache[2] = (seq_t)90;
    cache[3] = (seq_t)202;
    cache[4] = (seq_t)478;

    last_filled = 4;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 0 */

    init_base();

    if (n <= last_filled) return cache[n];

    /* grow cache to accommodate n */
    ensure_cache(n);

    /* compute missing values */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        seq_t term =
            cache[i - 1] * (seq_t)2
          + cache[i - 2] * (seq_t)6
          - cache[i - 3] * (seq_t)10
          - cache[i - 4] * (seq_t)5;
        cache[i] = term;
    }
    last_filled = n;

    return cache[n];
}

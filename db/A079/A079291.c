/*
 * A079291  Squares of Pell numbers.
 * https://oeis.org/A079291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=4; a(n)=5*a(n-1)+5*a(n-2)-a(n-3) (n>=3) –
 *            squares of Pell numbers
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;      /* dynamically allocated array of terms   */
static size_t cache_cap  = 0;         /* number of elements allocated           */
static idx_t  max_computed = 0;       /* largest index that has been filled */

/* Ensure the cache can hold at least n+1 elements (and at least 3 for the base terms). */
static void ensure_capacity(idx_t n)
{
    size_t needed = n + 1;
    if (needed < 3)            /* always reserve space for a(0..2) */
        needed = 3;
    if (needed <= cache_cap)
        return;                /* already big enough */

    size_t new_cap = needed;
    if (cache_cap != 0)
        new_cap = cache_cap * 2 > needed ? cache_cap * 2 : needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {               /* allocation failed – leave cache NULL; caller will return 0 */
        cache = NULL;
        cache_cap = 0;
        return;
    }
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Compute a(n) using the third‑order recurrence. */
seq_t a(idx_t n)
{
    /* Adjust cache size to accommodate the requested index. */
    ensure_capacity(n);

    /* Initialise the base terms exactly once. */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        max_computed = 2;
    }

    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 5*a(i-1) + 5*a(i-2) - a(i-3) */
        cache[i] = 5 * cache[i - 1] + 5 * cache[i - 2] - cache[i - 3];
    }
    max_computed = n;
    return cache[n];
}

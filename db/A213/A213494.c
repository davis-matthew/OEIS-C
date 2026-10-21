/*
 * A213494  a(n) = A213493(n)/12.
 * https://oeis.org/A213494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)-a(n-10) Initial
 *            terms: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1,
 *            a(7)=4, a(8)=8, a(9)=17 (valid for all n >= 0)
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values a(0)…a(sz-1) */
static size_t cap   = 0;         /* allocated capacity (number of slots) */
static size_t sz    = 0;         /* lowest index not yet computed (== cache size) */

/* Initialise the cache with the first ten terms.  Called lazily. */
static void init_cache_if_needed(void)
{
    if (cache != NULL)
        return;

    cap = 10;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (cache == NULL)
        abort(); /* allocation failed */

    static const seq_t init[10] = {0,0,0,0,0,0,1,4,8,17};
    for (size_t i = 0; i < 10; ++i)
        cache[i] = init[i];

    sz = 10;
}

/* Ensure cache can store indices up to 'n' inclusive. */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap)
        return;

    size_t newcap = cap;
    while (newcap < needed)
        newcap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (tmp == NULL)
        abort(); /* allocation failed */

    cache = tmp;
    cap   = newcap;
}

/* Compute a(n) using memoisation and the recurrence. */
seq_t a(idx_t n)
{
    init_cache_if_needed();

    /* Request is for one of the first ten terms: just return it. */
    if (n < 10)
        return cache[(size_t)n];

    ensure_capacity(n);

    /* Compute any missing values up to n. */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t v = cache[i-1] + cache[i-2] + cache[i-8] + cache[i-9];
        v -= 2 * cache[i-5];
        v -= cache[i-10];
        cache[i] = v;
    }
    sz = (size_t)n + 1;

    return cache[(size_t)n];
}

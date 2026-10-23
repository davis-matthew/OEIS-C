/*
 * A126354  a(n) = 6*a(n-2) - a(n-4) for n > 4, with a(1)=1, a(2)=0, a(3)=3,
 *          a(4)=2.
 * https://oeis.org/A126354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) for n>4, with a(1)=1, a(2)=0, a(3)=3,
 *            a(4)=2. Offset: 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t   cache_size   = 0;  /* number of terms stored */
static idx_t   cache_capacity = 0;/* allocated capacity */

/* Ensure that the cache can hold at least `need` elements. */
static void ensure_cache_size(idx_t need)
{
    if (need <= cache_capacity)
        return;
    idx_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_capacity = new_cap;
}

/* Populate the cache with the first four (and only) pre‑defined terms. */
static void init_cache(void)
{
    if (cache_size > 0)
        return;
    ensure_cache_size(4);
    cache[0] = (seq_t)1;  /* a(1) */
    cache[1] = (seq_t)0;  /* a(2) */
    cache[2] = (seq_t)3;  /* a(3) */
    cache[3] = (seq_t)2;  /* a(4) */
    cache_size = 4;
}

/* Return the nth term of A126354 (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behavior below the offset is unspecified */

    init_cache();

    if (n <= cache_size)
        return cache[n-1];

    ensure_cache_size(n);
    for (idx_t i = cache_size; i < n; ++i)
    {
        /* i   : 0‑based index of the element being computed
         * k   : i+1
         * a(k) = 6*a(k-2) - a(k-4)
         *       = 6*cache[k-3] - cache[k-5]
         *       = 6*cache[i-2] - cache[i-4]          (since i=k-1)
         */
        seq_t term = (seq_t)6 * cache[i-2] - cache[i-4];
        cache[i] = term;
    }
    cache_size = n;
    return cache[n-1];
}

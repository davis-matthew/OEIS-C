/*
 * A208212  a(n) = (a(n-1)^2*a(n-2)^5+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^2 * a(n-2)^5 + 1) / a(n-3) with a(0) = a(1) =
 *            a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache to memoise computed terms.
   cache_len : number of terms currently stored (first n terms, n == cache_len)
   cache_cap : current capacity of the cache array              */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure that the cache is large enough for at least `needed` elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) { /* fall back to abort */ abort(); }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute a(n) as defined above.  The recursion has the **Laurent
   property**, so the division always yields an integer. */
seq_t a(idx_t n)
{
    /* n < 3 gives the initial values. */
    if (n < 3)
        return 1;

    /* Convert idx_t to size_t for array indexing (safe because n >= 3). */
    size_t idx = (size_t)n;

    /* Initialise cache if needed. */
    if (cache_len == 0)
    {
        ensure_cache_cap(4);          /* at least space for a0..a3 */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 3;
    }

    /* If the requested term is already computed, return it. */
    if (idx < cache_len)
        return cache[idx];

    /* Grow cache to accommodate up to idx. */
    ensure_cache_cap(idx + 1);

    /* Compute missing terms sequentially up to idx. */
    for (size_t i = cache_len; i <= idx; ++i)
    {
        /* a(i) depends on a(i-1), a(i-2), a(i-3). All of these
           indices are already available because i >= 3. */
        seq_t t1 = cache[i - 1];
        t1 *= t1;                               /* a(i-1)^2 */

        seq_t t2 = cache[i - 2];
        /* Compute a(i-2)^5 by repeated multiplication. */
        seq_t t2pow = t2;
        t2pow *= t2;   /* t2^2 */
        t2pow *= t2;   /* t2^3 */
        t2pow *= t2;   /* t2^4 */
        t2pow *= t2;   /* t2^5 */

        seq_t prod = t1 * t2pow;                /* a(i-1)^2 * a(i-2)^5 */
        seq_t numer = prod + 1;                 /* +1 */
        seq_t denom = cache[i - 3];
        cache[i] = numer / denom;               /* integer division */
    }

    cache_len = idx + 1;
    return cache[idx];
}

/*
 * A212757  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          2w=max{w,x,y,z}-min{w,x,y,z}.
 * https://oeis.org/A212757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the linear recurrence a(n) =
 *            -a(n-1)+a(n-2)+4*a(n-3)+3*a(n-4)-3*a(n-5)-6*a(n-6)-3*a(n-7)
 *            +3*a(n-8)+4*a(n-9)+a(n-10)-a(n-11)-a(n-12) with initial values
 *            a(0)=1, a(1)=1, a(2)=13, a(3)=32, a(4)=56, a(5)=80, a(6)=177,
 *            a(7)=213, a(8)=297, a(9)=472, a(10)=580, a(11)=688
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cached terms */
static idx_t  cache_len = 0;    /* allocated size */
static idx_t  cache_last = 0;   /* index of largest computed term */

/* Initialise the cache with the first 12 terms.  The routine is called
   only once, on the first invocation of a(). */
static void init_cache(void)
{
    const seq_t ini[12] = {
        1, 1, 13, 32, 56, 80, 177, 213, 297, 472, 580, 688
    };
    cache_len = 12;
    cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
    if (!cache) abort();
    for (idx_t i = 0; i < 12; ++i)
        cache[i] = ini[i];
    cache_last = 11;
}

/* Ensure that cache has room for indices up to needed. */
static void ensure_capacity(idx_t needed)
{
    if (needed + 1 > cache_len) {
        idx_t new_len = needed + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }
}

/* Return the nth term of sequence A212757. */
seq_t a(idx_t n)
{
    /* first call: initialise the cache with the 12 seed values */
    if (cache == NULL)
        init_cache();

    /* if the element is already known, just return it */
    if (n <= cache_last)
        return cache[n];

    /* otherwise make the cache large enough and compute all missing terms */
    ensure_capacity(n);
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* compute a(i) using the recurrence, avoiding negative
           intermediates by separating positive and negative
           contributions. */
        seq_t pos = cache[i-2];
        pos += cache[i-3] * (seq_t)4;
        pos += cache[i-4] * (seq_t)3;
        pos += cache[i-8] * (seq_t)3;
        pos += cache[i-9] * (seq_t)4;
        pos += cache[i-10];

        seq_t neg = cache[i-1];
        neg += cache[i-5] * (seq_t)3;
        neg += cache[i-6] * (seq_t)6;
        neg += cache[i-7] * (seq_t)3;
        neg += cache[i-11];
        neg += cache[i-12];

        /* the recurrence guarantees pos >= neg, so subtraction is safe */
        cache[i] = pos - neg;
    }
    cache_last = n;
    return cache[n];
}

/*
 * A087844  a(n) = a(a(a(min(n-1, abs(n - a(n-1)))))) + a(abs(n - a(n-3))).
 * https://oeis.org/A087844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(a(min(n-1, abs(n - a(n-1))))) ) + a(abs(n - a(n-3))),
 *            with a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 645, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;               /* memoised values */
static unsigned char *cached = NULL;      /* 0 = not computed, 1 = computed */
static idx_t cache_size = 0;              /* current allocated size */

/* Ensure that indices up to n are supported.  The array grows geometrically. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;                      /* already big enough */

    idx_t new_size = cache_size ? cache_size << 1 : 16;
    while (new_size <= n) new_size <<= 1;             /* need > n */

    seq_t *c = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!c) abort();
    unsigned char *cf = (unsigned char *)realloc(cached, new_size);
    if (!cf) abort();

    cache = c;
    cached = cf;
    /* Newly allocated slots are uncomputed by default. */
    for (idx_t i = cache_size; i < new_size; ++i) cached[i] = 0;
    cache_size = new_size;
}

/* Recursive computation with memoisation. */
seq_t a(idx_t n)
{
    if (n <= 3) return (seq_t)1;                     /* a(1)=a(2)=a(3)=1 */

    ensure_cache(n);
    if (cached[n]) return cache[n];

    /* a(n-1) */
    seq_t an1 = a(n-1);

    /* diff1 = |n - a(n-1)| */
    seq_t diff1 = (seq_t)n > an1 ? (seq_t)n - an1 : an1 - (seq_t)n;

    /* minIndex = min(n-1, diff1) as an idx_t */
    idx_t minIndex;
    if (diff1 > (seq_t)(n-1)) minIndex = n-1;
    else                      minIndex = (idx_t)diff1;

    /* t1 = a(minIndex), t2 = a(t1), t3 = a(t2) */
    seq_t t1 = a(minIndex);
    seq_t t2 = a((idx_t)t1);
    seq_t t3 = a((idx_t)t2);

    /* a(n-3) */
    seq_t an3 = a(n-3);

    /* diff2 = |n - a(n-3)| */
    seq_t diff2 = (seq_t)n > an3 ? (seq_t)n - an3 : an3 - (seq_t)n;

    /* t4 = a(diff2) */
    seq_t t4 = a((idx_t)diff2);

    seq_t result = t3 + t4;

    cache[n] = result;
    cached[n] = 1;
    return result;
}

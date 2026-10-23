/*
 * A192376  Constant term of the reduction by x^2->x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=0, a(3)=7, a(4)=16
 *            a(n)=2*a(n-1)+6*a(n-2)+2*a(n-3)-a(n-4) for n>=5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for previously computed terms.
   cache_p[i] holds a(i+1).  cache_len is the number of valid entries.
   cache_alloc is the total allocated capacity. */
static seq_t *cache_p = NULL;
static size_t cache_len  = 0;
static size_t cache_alloc = 0;

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (cache_alloc >= needed)
        return;

    size_t new_alloc = cache_alloc ? cache_alloc : 4;
    while (new_alloc < needed)
        new_alloc <<= 1;          /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache_p, new_alloc * sizeof(seq_t));
    if (!tmp)
        abort();                  /* out of memory */

    cache_p    = tmp;
    cache_alloc = new_alloc;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined below offset */

    size_t N = (size_t)n;

    /* Initialize first four values if not yet done. */
    if (cache_len < 4) {
        ensure_cache(4);
        cache_p[0] = (seq_t)1;   /* a(1) */
        cache_p[1] = (seq_t)0;   /* a(2) */
        cache_p[2] = (seq_t)7;   /* a(3) */
        cache_p[3] = (seq_t)16;  /* a(4) */
        cache_len = 4;
    }

    /* Compute any new terms up to N. */
    if (N > cache_len) {
        ensure_cache(N);
        for (size_t i = cache_len; i < N; ++i) {
            seq_t t1 = cache_p[i-1];
            seq_t t2 = cache_p[i-2];
            seq_t t3 = cache_p[i-3];
            seq_t t4 = cache_p[i-4];
            cache_p[i] =
                (seq_t)(( (seq_t)2 * t1) + ((seq_t)6 * t2)
                        + ((seq_t)2 * t3) - t4);
        }
        cache_len = N;
    }

    return cache_p[N-1];
}

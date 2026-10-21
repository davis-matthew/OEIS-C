/*
 * A211522  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w + 5y = 2x.
 * https://oeis.org/A211522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-5)-a(n-6)-a(n-7)+a(n-8) a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=3, a(6)=4, a(7)=6, a(8)=8
 * Range:     u8 n <= 41, u16 n <= 661, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;         /* Cached terms 0 .. cache_size-1   */
static size_t cache_size = 0;            /* Number of terms currently stored */
static size_t cache_cap  = 0;            /* Allocated capacity                */

/* Initial values – the first nine terms. */
static const seq_t init_terms[9] = {0,0,0,1,2,3,4,6,8};

/* Make sure the cache can store indices 0..n (inclusive).     */
static void
ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < (size_t)(n + 1))
        newcap <<= 1;                        /* grow geometrically */

    if (cache == NULL)
        cache = (seq_t *)calloc(newcap, sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));

    cache_cap = newcap;
}

/* Return the number of ordered triples (w,x,y) with all terms in
   {1,…,n} and w + 5y = 2x.  The function works for any n >= 0
   and supports calls in arbitrary order. */
seq_t
a(idx_t n)
{
    if (n >= cache_size) {
        ensure_capacity(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            if ((size_t)i < 9)
                cache[i] = init_terms[i];
            else {
                seq_t t1 = cache[i-1];
                seq_t t2 = cache[i-2];
                seq_t t3 = cache[i-3];
                seq_t t5 = cache[i-5];
                seq_t t6 = cache[i-6];
                seq_t t7 = cache[i-7];
                seq_t t8 = cache[i-8];
                cache[i] = t1 + t2 - t3 + t5 - t6 - t7 + t8;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}

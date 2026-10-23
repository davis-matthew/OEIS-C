/*
 * A008622  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A008622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-6)+a(n-7)-a(n-10)-a(n-11)-a(n-13)+a(n-17)
 *            Initial values:
 *            a(0)=1,a(1)=0,a(2)=0,a(3)=0,a(4)=1,a(5)=0,a(6)=1,a(7)=1,
 *            a(8)=1,a(9)=0,a(10)=1,a(11)=1,a(12)=2,a(13)=1,a(14)=2,a(15)=1,a(16)=2,a(17)=1
 * Range:     u8 n <= 281, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache */
static idx_t  cache_max = 0;   /* highest index computed */

/* Initialise cache with known initial terms up to a(17) */
static void cache_init(void)
{
    size_t init_cap = 64; /* initial allocation size, must be >= 18 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return;                /* allocation failure – caller may ignore */
        for (size_t i = 0; i < init_cap; ++i) cache[i] = 0;
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 0;
        cache[6] = 1;
        cache[7] = 1;
        cache[8] = 1;
        cache[9] = 0;
        cache[10] = 1;
        cache[11] = 1;
        cache[12] = 2;
        cache[13] = 1;
        cache[14] = 2;
        cache[15] = 1;
        cache[16] = 2;
        cache[17] = 1;
        cache_cap = init_cap;
        cache_max = 17;
    }
}

/* Ensure cache has capacity for indices up to n (inclusive) */
static void cache_ensure(idx_t n)
{
    cache_init();
    if ((size_t)n + 1 <= cache_cap) return;        /* already big enough */
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) return;                      /* ignore failure */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute terms from current maximum up to n */
static void compute_terms(idx_t n)
{
    if (n <= cache_max) return;              /* nothing to do */
    idx_t start = cache_max + 1;
    if (start < 18) start = 18;              /* recurrence starts at n = 18 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t pos = cache[i-4] + cache[i-6] + cache[i-7] + cache[i-17];
        seq_t neg = cache[i-10] + cache[i-11] + cache[i-13];
        cache[i] = pos - neg;                /* pos >= neg for all n */
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    cache_ensure(n);          /* make sure array is large enough */
    compute_terms(n);         /* compute up to n if needed */
    return cache[n];
}

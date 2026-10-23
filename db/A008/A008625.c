/*
 * A008625  G.f.: (1+x^3)*(1+x^5)*(1+x^6)/((1-x^4)*(1-x^6)*(1-x^7)) (or
 *          (1+x^5)(1+x^6)/((1-x^3)*(1-x^4)*(1-x^7))).
 * https://oeis.org/A008625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Sequence A008625: a(n) = a(n-3) + a(n-7) - a(n-10) + 1   (n ≥ 10)
 *  with initial values (offset 0):
 *  a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=2, a(8)=2, a(9)=3
 *
 *  The function uses a static cache for memoisation and computes new terms on demand.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    const idx_t base_size = 10; /* indices 0 .. 9 */

    /* initialise cache with first 10 terms if not yet done */
    if (cache == NULL) {
        cache = (seq_t *)malloc(base_size * sizeof(seq_t));
        /* defined in the OEIS entry */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)2;
        cache[7] = (seq_t)2;
        cache[8] = (seq_t)2;
        cache[9] = (seq_t)3;
        cache_size = base_size;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to hold up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0; /* out of memory – failure mode */
    cache = tmp;

    /* compute terms from current cache size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)1; /* +1 term */
        if (i >= 3) val += cache[i - 3];
        if (i >= 7) val += cache[i - 7];
        if (i >= 10) val -= cache[i - 10];
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}

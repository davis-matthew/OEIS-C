/*
 * A196573  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,2,3,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3*a(n-3) + 5*a(n-4) - a(n-6),
 *            a(1)=1, a(2)=3, a(3)=16, a(4)=37, a(5)=117, a(6)=372
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 is unused & set to 0, indices 1..6 contain
       the initial values; cache_len is the current allocated length */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache on first use */
    if (!cache) {
        /* allocate space for indices 0..6 (7 elements) */
        cache_len = 7;
        cache = (seq_t*)calloc((size_t)cache_len, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, return 0 */
        /* cache[0] is left zero for simplicity */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)37;
        cache[5] = (seq_t)117;
        cache[6] = (seq_t)372;
    }

    /* return immediately if already computed */
    if (n < cache_len) return cache[n];

    /* grow cache up to index n */
    idx_t new_len = n + 1; /* we need indices 0..n inclusive */
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) return 0;                     /* allocation failure */
    cache = tmp;
    /* zero new elements – important for the added space */
    for (idx_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
    cache_len = new_len;

    /* compute missing values using the recurrence */
    for (idx_t i = 7; i <= n; ++i) {      /* i>=7 because recurrence needs i-6 */
        cache[i] = (  (seq_t)2 * cache[i-1]
                    + (seq_t)2 * cache[i-2]
                    + (seq_t)3 * cache[i-3]
                    + (seq_t)5 * cache[i-4]
                    -       cache[i-6] );
    }

    return cache[n];
}

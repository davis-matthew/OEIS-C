/*
 * A176828  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=2, k=1 and l=-1.
 * https://oeis.org/A176828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176828 ----------------------------------------------------------
   Recurrence:  a(0) = 1,  a(1) = 2
   For n >= 1:
      a(n+1) = Σ_{p=0}^{n} (a(p) * a(n-p) + k) + l
   with k = 1 and l = -1.
   Thus: a(n+1) = Σ_{p=0}^{n} a(p) * a(n-p) + n      (n >= 1)
   --------------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_size    = 0;     /* memory allocated (number of slots)      */
    static idx_t  computed_upto = 0;     /* highest index that has a computed value */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate few elements; will grow when needed */
        idx_t init = (n > 4) ? n : 4;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return 0;             /* out of memory – unlikely in tests */
        cache[0] = 1;                     /* a(0) */
        cache[1] = 2;                     /* a(1) */
        cache_size = init;
        computed_upto = 1;                /* we have a[0] and a[1] ready */
    }

    /* enlarge cache if the requested index exceeds current capacity */
    if (cache_size <= n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;             /* out of memory – unlikely */
        cache = tmp;
        cache_size = new_sz;
    }

    /* compute terms up to the requested index, if not done yet */
    while (computed_upto < n) {
        idx_t next_idx = computed_upto + 1;   /* we will compute a[next_idx]   */
        seq_t sum = 0;

        /* convolution part: Σ a(p) * a[next_idx-1-p]  (p = 0..next_idx-1) */
        for (idx_t p = 0; p <= next_idx - 1; ++p) {
            sum += cache[p] * cache[next_idx - 1 - p];
        }

        /* add the linear term: n = next_idx-1  (since a(n+1) needs n) */
        sum += (seq_t)(next_idx - 1);

        cache[next_idx] = sum;
        ++computed_upto;                    /* mark this index as computed   */
    }

    return cache[n];
}

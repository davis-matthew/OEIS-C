/*
 * A232475  Number of preferential arrangements of n labeled elements when at
 *          least k=4 elements per rank are required.
 * https://oeis.org/A232475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=4..n} binomial(n,k) * a(n-k) ; initial values:
 *            a(0)=1, a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 17, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"    /* binomial(idx_t, idx_t) → seq_t */

seq_t a(idx_t n)
{
    /* static cache with flags indicating computed entries */
    static seq_t *cache   = NULL;          /* stored values of a(i) */
    static unsigned char *comp  = NULL;    /* 1 if a(i) computed */
    static idx_t capacity = 0;             /* current size of cache arrays */

    /* Allocate or grow cache to hold indices up to n */
    if ((size_t)n >= capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        unsigned char *new_comp = (unsigned char *)malloc(new_cap * sizeof(unsigned char));
        if (!new_cache || !new_comp) return 0;  /* out of memory, caller handles */

        if (cache) {
            memcpy(new_cache, cache, capacity * sizeof(seq_t));
            memcpy(new_comp, comp, capacity * sizeof(unsigned char));
        }
        memset(new_cache + capacity, 0, (new_cap - capacity) * sizeof(seq_t));
        memset(new_comp + capacity, 0, (new_cap - capacity) * sizeof(unsigned char));

        free(cache);
        free(comp);
        cache = new_cache;
        comp  = new_comp;
        capacity = new_cap;
    }

    /* Return cached value if available */
    if (comp[n]) return cache[n];

    /* Base cases */
    if (n == 0) {
        cache[0] = 1;
        comp[0] = 1;
        return 1;
    }
    if (n < 4) {
        cache[n] = 0;
        comp[n] = 1;
        return 0;
    }

    /* Compute a(n) using the recurrence, ensuring sub‑values are computed */
    seq_t sum = 0;
    for (idx_t k = 4; k <= n; ++k) {
        seq_t b = binomial(n, k);      /* C(n,k) */
        seq_t t = b * a(n - k);        /* recursive call guarantees a(n-k) computed */
        sum += t;
    }

    cache[n] = sum;
    comp[n]  = 1;
    return sum;
}

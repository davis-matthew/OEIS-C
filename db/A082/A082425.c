/*
 * A082425  a(1)=1, a(n) = -1 + n*Sum_{j=1..n-1} a(j).
 * https://oeis.org/A082425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n) = -1 + n*Sum_{j=1}^{n-1} a(j)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_alloc = 0;   /* largest index allocated + 1 */
    static seq_t sum_total = 0;     /* Sum_{j=1}^{k} a(j) for the last computed k */
    static idx_t computed = 0;      /* last index for which a(k) has been computed */

    if (n == 0) return 0;               /* safety guard; not used in the OEIS offset */

    /* Grow the cache if necessary */
    if (n > cache_alloc) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* malloc / realloc failure is not handled – it will crash on purpose */
        cache = tmp;
        cache_alloc = n;
    }

    /* Initialize the first term if this is the very first call */
    if (computed == 0) {
        cache[1] = (seq_t)1;
        sum_total = cache[1];       /* sum of a(1) only */
        computed = 1;
    }

    /* Compute terms that are still missing */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t val = i_seq * sum_total - (seq_t)1;
        cache[i] = val;
        sum_total += val;
    }
    computed = n;

    return cache[n];
}

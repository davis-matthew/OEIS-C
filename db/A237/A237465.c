/*
 * A237465  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          lower median minus the upper median of every 2 X 2 subblock
 *          differing from its horizontal and vertical neighbors by exactly
 *          one.
 * https://oeis.org/A237465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 288*a(n-4) + 1840*a(n-6) + 784*a(n-8) with a(1)=81,
 *            a(2)=356, a(3)=1566, a(4)=6954, a(5)=30913, a(6)=136488,
 *            a(7)=603184, a(8)=2671904
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static const seq_t init[9] = {
        (seq_t)0,           /* index 0 unused */
        (seq_t)81,
        (seq_t)356,
        (seq_t)1566,
        (seq_t)6954,
        (seq_t)30913,
        (seq_t)136488,
        (seq_t)603184,
        (seq_t)2671904
    };

    if (n <= 8)   /* return the base value */
        return init[n];

    /* initialise cache on first use */
    if (!cache) {
        cache_len = 9;
        cache = alloc_seq_array(cache_len);
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* extend cache if needed */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = alloc_seq_array(new_len);

        /* copy already known terms */
        for (idx_t i = 0; i < old_len; ++i)
            tmp[i] = cache[i];

        /* compute the missing terms using the recurrence */
        for (idx_t i = old_len; i <= n; ++i)
            tmp[i] = (seq_t)288 * tmp[i-4] +
                     (seq_t)1840 * tmp[i-6] +
                     (seq_t)784  * tmp[i-8];

        free_seq_array(cache);
        cache = tmp;
        cache_len = new_len;
    }

    return cache[n];
}

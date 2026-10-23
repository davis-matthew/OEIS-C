/*
 * A028096  Expansion of 1/((1-3x)(1-7x)(1-10x)(1-11x)).
 * https://oeis.org/A028096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (21*11^(n+3)-32*10^(n+3)+2*7^(n+4)-3^(n+4))/672
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Base values for n = 0,1,2,3 */
    static const seq_t init[4] = {
        (seq_t)1,
        (seq_t)31,
        (seq_t)620,
        (seq_t)10190
    };

    /* Cached terms from previous calls */
    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;   /* largest index that is currently cached */
    static idx_t capacity   = 0;    /* current allocated capacity */

    /* Handle small indices directly */
    if (n < 4) {
        return init[n];
    }

    /* Ensure that the first four terms are in the cache */
    if (cached_upto < 3) {
        idx_t need = 4;
        if (capacity < need) {
            cache = seq_realloc(cache, capacity, need);
            capacity = need;
        }
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cached_upto = 3;
    }

    /* Extend the cache up to n if necessary */
    if (n > cached_upto) {
        idx_t desired = n + 1;
        if (capacity < desired) {
            cache = seq_realloc(cache, capacity, desired);
            capacity = desired;
        }
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            /* Recurrence: a(n) = 31*a(n-1) - 341*a(n-2) + 1541*a(n-3) - 2310*a(n-4) */
            seq_t t1 = (seq_t)31   * cache[i - 1];
            seq_t t2 = (seq_t)341  * cache[i - 2];
            seq_t t3 = (seq_t)1541 * cache[i - 3];
            seq_t t4 = (seq_t)2310 * cache[i - 4];
            seq_t val = t1 + t3;   /* accumulate positive terms first */
            val -= t2;              /* subtraction is safe because intermediate sum is larger */
            val -= t4;
            cache[i] = val;
        }
        cached_upto = n;
    }

    return cache[n];
}

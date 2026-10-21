/*
 * A224185  Number of n X 3 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (n > 7) Initial values: a(1)=22, a(2)=158,
 *            a(3)=684, a(4)=2205, a(5)=5852, a(6)=13524, a(7)=28176
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 62, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[7] = {
    (seq_t)22, (seq_t)158, (seq_t)684, (seq_t)2205,
    (seq_t)5852, (seq_t)13524, (seq_t)28176
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)n);
    if (!new_cache)  /* allocation failure: leave cache unchanged */
        return;
    cache = new_cache;
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 7) {
            cache[i] = init_vals[i];
        } else {
            seq_t t1 = (seq_t)7  * cache[i-1];
            seq_t t2 = (seq_t)21 * cache[i-2];
            seq_t t3 = (seq_t)35 * cache[i-3];
            seq_t t4 = (seq_t)35 * cache[i-4];
            seq_t t5 = (seq_t)21 * cache[i-5];
            seq_t t6 = (seq_t)7  * cache[i-6];
            cache[i] = t1 - t2 + t3 - t4 + t5 - t6 + cache[i-7];
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* out of bounds, safe fallback */
    ensure_cache(n);
    return cache[n-1];
}

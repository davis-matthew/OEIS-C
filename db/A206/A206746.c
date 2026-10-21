/*
 * A206746  Number of (n+1) X 3 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an equal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A206746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*a(n-1) -489*a(n-2) +2516*a(n-3) -5247*a(n-4) *
 *            +276*a(n-5) +15809*a(n-6) -23256*a(n-7) +11000*a(n-8) *
 *            +184*a(n-9) -832*a(n-10) * Initial values: a(1)=1888,
 *            a(2)=36384, a(3)=793152, * a(4)=18037344, a(5)=415429504, *
 *            a(6)=9604042208, a(7)=222273709440, * a(8)=5145902199776,
 *            a(9)=1191449...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cached_n = 0;
static size_t cache_capacity = 0;

static void init_cache(void) {
    if (!cache) {
        cache_capacity = 20;
        cache = (seq_t *)calloc(cache_capacity + 1, sizeof(seq_t));
        /* 0‑based index unused */
        cache[1]  = (seq_t)1888;
        cache[2]  = (seq_t)36384;
        cache[3]  = (seq_t)793152;
        cache[4]  = (seq_t)18037344;
        cache[5]  = (seq_t)415429504;
        cache[6]  = (seq_t)9604042208;
        cache[7]  = (seq_t)222273709440;
        cache[8]  = (seq_t)5145902199776;
        cache[9]  = (seq_t)119144947541120;
        cache[10] = (seq_t)2758681735263072;
        cached_n = 10;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n ≤ 0 */
    init_cache();

    if (n <= cached_n) return cache[n];

    /* Grow the cache if necessary */
    if ((idx_t)cache_capacity < n) {
        size_t new_cap = cache_capacity ? cache_capacity : 64;
        while ((idx_t)new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        /* zero the newly allocated space */
        size_t added = new_cap - cache_capacity;
        if (added > 0) {
            seq_t *ptr = tmp + cache_capacity + 1;
            for (size_t i = 0; i < added; ++i) ptr[i] = (seq_t)0;
        }
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t p = 0, q = 0;
        /* Positive contributions */
        p += (seq_t)40   * cache[i - 1];
        p += (seq_t)2516 * cache[i - 3];
        p += (seq_t)276  * cache[i - 5];
        p += (seq_t)15809 * cache[i - 6];
        p += (seq_t)11000 * cache[i - 8];
        p += (seq_t)184   * cache[i - 9];
        /* Negative contributions */
        q += (seq_t)489   * cache[i - 2];
        q += (seq_t)5247  * cache[i - 4];
        q += (seq_t)23256 * cache[i - 7];
        q += (seq_t)832   * cache[i - 10];
        cache[i] = p - q;
    }
    cached_n = n;
    return cache[n];
}

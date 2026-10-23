/*
 * A164619  Integers of the form A164577(k)/3.
 * https://oeis.org/A164619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = +2*a(n-1) -a(n-2) -a(n-3) +2*a(n-4) -a(n-5) +2*a(n-6)
 *            -4*a(n-7) +2*a(n-8) +2*a(n-9) -4*a(n-10) +2*a(n-11) -a(n-12)
 *            +2*a(n-13) -a(n-14) -a(n-15) +2*a(n-16) -a(n-17) Initial terms:
 *            a(1)=4, a(2)=15, a(3)=54, a(4)=75, a(5)=132, a(6)=169, a(7)=320,
 *            a(8)=459, a(9)=735, a(10)=847, a(11)=1104, a(1...
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#endif

static const seq_t init_vals[17] = {
    (seq_t)4,   (seq_t)15,  (seq_t)54,   (seq_t)75,   (seq_t)132,
    (seq_t)169, (seq_t)320, (seq_t)459,  (seq_t)735,  (seq_t)847,
    (seq_t)1104,(seq_t)1250,(seq_t)1764, (seq_t)2175,(seq_t)2904,
    (seq_t)3179,(seq_t)3780
};

static int initialized = 0;

#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
#endif

static void init_cache(void) {
    if (initialized) return;
    initialized = 1;
#ifdef __cplusplus
    cache.reserve(17);
    for (int i = 0; i < 17; ++i) cache.push_back(init_vals[i]);
#else
    cache_capacity = 17;
    cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
    for (size_t i = 0; i < 17; ++i) cache[i] = init_vals[i];
#endif
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety guard for n < 1 */

    init_cache();

    /* conversion for array indexing */
    size_t idx = (size_t)n - 1;

#ifdef __cplusplus
    size_t len = cache.size();
    if (idx < len) return cache[idx];

    size_t needed = (size_t)n;
    cache.resize(needed);

    for (size_t i = len; i < needed; ++i) {
        seq_t val =  2 * cache[i-1] - cache[i-2] - cache[i-3]
                 + 2 * cache[i-4] - cache[i-5] + 2 * cache[i-6]
                 - 4 * cache[i-7] + 2 * cache[i-8] + 2 * cache[i-9]
                 - 4 * cache[i-10] + 2 * cache[i-11] - cache[i-12]
                 + 2 * cache[i-13] - cache[i-14] - cache[i-15]
                 + 2 * cache[i-16] - cache[i-17];
        cache[i] = val;
    }
#else
    size_t len = cache_capacity;
    if (idx < len) return cache[idx];

    size_t needed = (size_t)n;
    if (needed > cache_capacity) {
        seq_t *new_cb = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache = new_cb;
        cache_capacity = needed;
    }

    for (size_t i = len; i < needed; ++i) {
        cache[i] =  2 * cache[i-1] - cache[i-2] - cache[i-3]
                  + 2 * cache[i-4] - cache[i-5] + 2 * cache[i-6]
                  - 4 * cache[i-7] + 2 * cache[i-8] + 2 * cache[i-9]
                  - 4 * cache[i-10] + 2 * cache[i-11] - cache[i-12]
                  + 2 * cache[i-13] - cache[i-14] - cache[i-15]
                  + 2 * cache[i-16] - cache[i-17];
    }
    len = cache_capacity;
#endif

    return cache[idx];
}

/*
 * A080522  Leading diagonal of triangle in A080521.
 * https://oeis.org/A080522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - (n-1)*(n-2)/2 - n + 1 (for n > 1), a(1) = 1
 *            Recurrence: a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4) for
 *            n > 5 Initial values: a(1)=1, a(2)=3, a(3)=5, a(4)=10, a(5)=22
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised. It works both in C11 and C++17.   */

#ifdef __cplusplus
#include <vector>
#include <cstring>
#else
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined, but avoid crash */

    /* static storage for memoisation */
#ifdef __cplusplus
    /* Use std::vector for C++ builds (supports arbitrary‑precision types) */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(6);        /* indices 0..5, 0 unused */
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 5;
        cache[4] = 10;
        cache[5] = 22;
        cache.resize(6);        /* reserve space for up to 5 terms initially */
    }

    /* Grow the vector as needed */
    while (cache.size() <= n) {
        /* compute next term using recurrence (valid for n>5) */
        seq_t t1 = 5 * cache[cache.size() - 1] + 7 * cache[cache.size() - 3];
        seq_t t2 = 9 * cache[cache.size() - 2] + 2 * cache[cache.size() - 4];
        cache.push_back(t1 - t2);
    }
    return cache[n];
#else
    /* C build: use a dynamic array (realloc) to avoid constructor issues */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t sz = 0;  /* highest filled index (cache[1..sz] valid) */

    /* initialise with first five terms */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t*)calloc(capacity + 1, sizeof(seq_t)); /* index 0 unused */
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 5;
        cache[4] = 10;
        cache[5] = 22;
        sz = 5;
    }

    /* grow array if necessary */
    while (n > capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap < n) new_cap = n;
        cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        capacity = new_cap;
    }

    /* compute missing terms */
    while (sz < n) {
        seq_t t1 = 5 * cache[sz] + 7 * cache[sz - 2];
        seq_t t2 = 9 * cache[sz - 1] + 2 * cache[sz - 3];
        cache[sz + 1] = t1 - t2;
        ++sz;
    }
    return cache[n];
#endif
}

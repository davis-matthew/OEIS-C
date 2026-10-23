/*
 * A173809  a(2*n+1) = 1+A131941(2*n+1). a(2*n) = A131941(2*n).
 * https://oeis.org/A173809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3, a(3)=9, a(4)=16,
 *            a(n)=3*a(n-1)-2*a(n-2)-2*a(n-3)+3*a(n-4)-a(n-5) for n>=5
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    static bool initialized = false;

    /* initialise cache once */
    if (!initialized) {
        /* initial capacity at least up to index 5 */
        idx_t need = 5 + 1; /* indices 0..5 */
        if (need > cache_cap) {
            cache_cap = need;
            cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)cache_cap);
            if (!cache) exit(EXIT_FAILURE);
        }
        /* set base terms */
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 9;
        cache[4] = 16;
        cache_len = 4; /* we have computed through a[4] */
        initialized = true;
    }

    /* trivial case */
    if (n <= cache_len)
        return cache[n];

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : n + 1;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *buf = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
        if (!buf) exit(EXIT_FAILURE);
        cache = buf;
        cache_cap = new_cap;
    }

    /* compute new terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 1];
        val -= 2 * cache[i - 2];
        val -= 2 * cache[i - 3];
        val += 3 * cache[i - 4];
        val -= cache[i - 5];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}

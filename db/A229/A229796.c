/*
 * A229796  Number of 3 X 3 0..n arrays with rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A229796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) for n > 10 Initial values: a(1)=45, a(2)=1169,
 *            a(3)=14178, a(4)=102251, a(5)=520017, a(6)=2066505,
 *            a(7)=6842284, a(8)=19692165, a(9)=50724037, a(10)=119421753
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 191, u128 n <= 207,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    /* initial constants (1‑based indexing, index 0 unused) */
    static const seq_t init_terms[11] = {
        0ULL,
        45ULL,
        1169ULL,
        14178ULL,
        102251ULL,
        520017ULL,
        2066505ULL,
        6842284ULL,
        19692165ULL,
        50724037ULL,
        119421753ULL
    };

    if (!cache) {
        /* allocate space for the first 10 terms plus a dummy 0 field */
        size_t init_size = 11;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_size);
        if (!cache)
            return 0;                     /* allocation failed */
        /* copy the known initial terms */
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = init_terms[i];
        cache[0] = 0;                     /* unused */
        cached_max = 10;
    }

    if (n <= cached_max)
        return cache[n];

    /* enlarge the cache to hold up to n */
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return 0;                         /* allocation failed */
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t val = 10 * cache[k - 1]
                  - 45 * cache[k - 2]
                  + 120 * cache[k - 3]
                  - 210 * cache[k - 4]
                  + 252 * cache[k - 5]
                  - 210 * cache[k - 6]
                  + 120 * cache[k - 7]
                  - 45 * cache[k - 8]
                  + 10  * cache[k - 9]
                  - cache[k - 10];
        cache[k] = val;
    }

    cached_max = n;
    return cache[n];
}

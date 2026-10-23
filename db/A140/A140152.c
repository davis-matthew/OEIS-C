/*
 * A140152  a(1)=1, a(n)=a(n-1)+n^3 if n odd, a(n)=a(n-1)+ n^0 if n is even.
 * https://oeis.org/A140152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + (n odd ? n^3 : 1) for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence is defined for n ≥ 1 */

    /* Static cache: cache[i] contains a(i), i ≥ 1 */
    static seq_t *cache = NULL;          /* array of computed values */
    static size_t capacity = 0;          /* maximum index currently stored  */
    static size_t last = 0;              /* highest index that has been computed */

    /* Ensure the cache is allocated */
    if (!cache) {
        capacity = (size_t)(n > 4 ? n : 4);          /* start with a small size */
        cache = (seq_t *)calloc(capacity + 1, sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failed */
        cache[1] = 1;           /* a(1) = 1 */
        last = 1;
    }
    /* Grow the cache if the requested index exceeds current capacity */
    else if ((size_t)n > capacity) {
        size_t new_cap = capacity ? capacity << 1 : 4;   /* double size or start at 4 */
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                       /* reallocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t delta;
        if ((i & 1ULL) == 0) {
            delta = 1;
        } else {
            seq_t ti = (seq_t)i;
            delta = ti * ti * ti;    /* i^3 */
        }
        cache[(size_t)i] = cache[(size_t)(i - 1)] + delta;
    }
    last = n;
    return cache[(size_t)n];
}

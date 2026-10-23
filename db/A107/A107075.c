/*
 * A107075  Centered square numbers that are also centered pentagonal numbers.
 * https://oeis.org/A107075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 322*a(n+1)-a(n)-140; a(1)=1, a(2)=181
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* a(n): compute the n-th term (1‑based) of A107075 */
seq_t a(idx_t n) {
    /* return 0 for indices outside the defined range */
    if (n < 1) return 0;

    /* static cache: index 0 unused */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* ensure capacity for indices up to n */
    if (cache_cap < n + 1) {
        idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first two terms */
    if (!cache[1]) cache[1] = (seq_t)1;
    if (!cache[2]) cache[2] = (seq_t)181;

    /* compute up to n if needed */
    for (idx_t i = 3; i <= n; ++i) {
        if (!cache[i]) {
            seq_t prev2 = cache[i - 2];
            seq_t prev1 = cache[i - 1];
            cache[i] = prev1 * (seq_t)322 - prev2 - (seq_t)140;
        }
    }

    return cache[n];
}

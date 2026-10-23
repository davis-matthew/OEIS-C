/*
 * A033500  a(n) = a(n-1) + a(round(2*(n-1)/3)) + a(round((n-1)/3)) with
 *          a(1)=1, a(2)=2.
 * https://oeis.org/A033500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(round(2*(n-1)/3)) + a(round((n-1)/3)), a(1) =
 *            1, a(2) = 2 (OEIS offset 1)
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 228, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation cache */
    static size_t cap   = 0;      /* allocated size of cache */

    /* Expand cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;       /* allocation failed – unlikely for n ≤ 10000 */
        for (size_t i = cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cap   = new_cap;
    }

    /* Return memoised value if already computed */
    if (cache[n] != 0) return cache[n];

    /* Base cases */
    if (n == 1) { cache[1] = 1; return 1; }
    if (n == 2) { cache[2] = 2; return 2; }

    /* Recursive computation */
    idx_t m  = n - 1;
    idx_t k1 = (2 * m + 1) / 3;  /* round(2*(n-1)/3) */
    idx_t k2 = (m + 1) / 3;      /* round((n-1)/3)  */

    seq_t val = a(m) + a(k1) + a(k2);
    cache[n] = val;
    return val;
}

/*
 * A075092  Sum of generalized tribonacci numbers (A001644) and reflected
 *          generalized tribonacci numbers (A073145).
 * https://oeis.org/A075092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + a(n-4) - a(n-6), a(0)=6, a(1)=0,
 *            a(2)=2, a(3)=12, a(4)=6, a(5)=20
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A075092. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static uint64_t cache_cap   = 0;   /* allocated capacity   */
    static uint64_t cache_size  = 0;   /* number of computed terms  */

    /* Ensure we have enough space for at least n+1 terms. */
    if (cache_cap <= (uint64_t)n) {
        uint64_t new_cap = cache_cap ? cache_cap * 2 : 6;
        if (new_cap < (uint64_t)n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 as a fallback */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms up to n. */
    for (uint64_t i = cache_size; i <= n; ++i) {
        seq_t value;
        switch (i) {
            case 0: value = 6; break;
            case 1: value = 0; break;
            case 2: value = 2; break;
            case 3: value = 12; break;
            case 4: value = 6; break;
            case 5: value = 20; break;
            default:
                /* a(n) = a(n-2) + 4*a(n-3) + a(n-4) - a(n-6) */
                value = cache[i-2] + (seq_t)4 * cache[i-3]
                        + cache[i-4] - cache[i-6];
                break;
        }
        cache[i] = value;
    }

    cache_size = n + 1;
    return cache[n];
}

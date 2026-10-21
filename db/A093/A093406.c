/*
 * A093406  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) + a(n-4).
 * https://oeis.org/A093406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n‑1) – 6*a(n‑2) + 4*a(n‑3) + a(n‑4) with a(1)=1,
 *            a(2)=3, a(3)=11, a(4)=31 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */
#include <stdint.h>   /* for uint64_t */

extern seq_t a(idx_t n)
{
    /* static cache for values up to a reasonably large N.
       The tests use n up to about 10000. */
    static seq_t cache[10001];   /* indices 0 … 10000 ==> n=1 … 10001 */
    static idx_t max_cached = 0; /* highest index cached (0‑based) */

    if (n == 0)   /* offset does not matter below 1 */
        return 0;

    /* ensure we can hold needed index */
    if (n > 10001) {          /* the cache is only sized for 10001 terms */
        /* For safety: grow the array if ever needed beyond 10001.
           This uses dynamic allocation that works for both C and C++. */
        static seq_t *dyn_cache = NULL;
        static idx_t dyn_size = 0;
        if (dyn_cache == NULL || n + 1 > dyn_size) {
            idx_t new_size = (n + 1) * 2;
            seq_t *tmp = (seq_t *)realloc(dyn_cache, new_size * sizeof(seq_t));
            if (!tmp) { /* allocation failed; return 0 as a fallback */
                return 0;
            }
            dyn_cache = tmp;
            dyn_size  = new_size;
        }
        /* compute up to n on the dynamic cache */
        for (idx_t i = max_cached + 1; i < n; ++i) {
            if (i < 4) {
                /* base terms */
                const seq_t base[4] = { (seq_t)1, (seq_t)3, (seq_t)11, (seq_t)31 };
                dyn_cache[i] = base[i];
            } else {
                dyn_cache[i] = 4 * dyn_cache[i-1]
                               - 6 * dyn_cache[i-2]
                               + 4 * dyn_cache[i-3]
                               + dyn_cache[i-4];
            }
        }
        max_cached = n - 1;
        return dyn_cache[n-1];
    }

    /* Use the static cache when possible. */
    if (n <= 4) {
        const seq_t base[4] = { (seq_t)1, (seq_t)3, (seq_t)11, (seq_t)31 };
        return base[n-1];
    }

    /* Initialize cache with the first four values if not already done. */
    if (max_cached < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)31;
        max_cached = 3;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_cached + 1; i < n; ++i) {
        cache[i] = 4 * cache[i-1]
                   - 6 * cache[i-2]
                   + 4 * cache[i-3]
                   + cache[i-4];
    }
    max_cached = n - 1;
    return cache[n-1];
}

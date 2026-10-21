/*
 * A207366  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +6*a(n-2) -14*a(n-3) -14*a(n-4) +42*a(n-5)
 *            +14*a(n-6) -70*a(n-7) +70*a(n-9) -14*a(n-10) -42*a(n-11)
 *            +14*a(n-12) +14*a(n-13) -6*a(n-14) -2*a(n-15) +a(n-16) a(1) =
 *            18, a(2) = 324, a(3) = 1350, a(4) = 5625, a(5) = 15525, a(6) =
 *            42849, a(7) = 95220, a(8) = 211600, a(9) = 409860, a(10)...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 35, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap      = 0;
    static idx_t max_computed = 0;

    if (n == 0)
        return 0;                /* undefined for n < 1, harmless placeholder */

    /* Allocate or resize cache to hold indices up to n (1‑based). */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 32 : cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache)
            return 0;            /* allocation failure – caller should handle */
        if (cache) {
            /* copy already computed values */
            for (size_t i = 0; i <= (size_t)max_computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* If value already computed (e.g., from a previous call), return it. */
    if (n <= max_computed)
        return cache[n];

    /* Base values for 1 ≤ n ≤ 16. */
    static const seq_t base[17] = {
        0ULL,          /* index 0 unused */
        18ULL, 324ULL, 1350ULL, 5625ULL, 15525ULL, 42849ULL,
        95220ULL, 211600ULL, 409860ULL, 793881ULL, 1397088ULL,
        2458624ULL, 4029760ULL, 6604900ULL, 10246590ULL, 15896169ULL
    };

    /* Compute all missing values up to n in ascending order. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 16) {
            cache[i] = base[i];
        } else {
            cache[i] =
                2ULL * cache[i-1] + 6ULL * cache[i-2]
                - 14ULL * cache[i-3] - 14ULL * cache[i-4]
                + 42ULL * cache[i-5] + 14ULL * cache[i-6]
                - 70ULL * cache[i-7] + 70ULL * cache[i-9]
                - 14ULL * cache[i-10] - 42ULL * cache[i-11]
                + 14ULL * cache[i-12] + 14ULL * cache[i-13]
                - 6ULL * cache[i-14] - 2ULL * cache[i-15]
                + cache[i-16];
        }
    }

    max_computed = n;
    return cache[n];
}

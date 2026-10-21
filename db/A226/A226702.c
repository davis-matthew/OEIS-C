/*
 * A226702  Positive solutions y/5 of the Pell equation x^2 - 61*y^2 = -4.
 * https://oeis.org/A226702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1522, a(n) = 1523*a(n-1) - a(n-2) for n >= 2
 *            (OEIS A226702)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of elements cached (>=2 after init) */
    static size_t cap = 0;      /* current allocated capacity */

    /* ensure at least 2 cached entries (0 and 1) */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) { /* memory allocation failure */
            exit(1);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1522;
        cached = 2;
    }

    /* expand cache if needed */
    size_t target = (size_t)n;
    if (target >= cached) {
        /* enlarge capacity if necessary */
        if (cap <= target) {
            size_t new_cap = cap ? cap * 2 : 2;
            while (new_cap <= target) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) {
                exit(1);
            }
            cache = tmp;
            cap = new_cap;
        }
        /* compute missing terms up to n */
        for (size_t i = cached; i <= target; ++i) {
            cache[i] = ((seq_t)1523 * cache[i - 1]) - cache[i - 2];
        }
        cached = target + 1;
    }

    return cache[(size_t)n];
}

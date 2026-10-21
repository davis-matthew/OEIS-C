/*
 * A231138  Number of (n+1) X (1+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal and vertical neighbors, with
 *          upper left element zero.
 * https://oeis.org/A231138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-3) + 4*a(n-4) with initial values
 *            a(1)=18, a(2)=100, a(3)=570, a(4)=3234 computed iteratively,
 *            memoizing results for any n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated length (index 0 .. cap-1) */
    static idx_t computed_up_to = 0; /* highest index computed so far */

    if (n < 1) return 0;                /* not expected but safe */

    /* ensure cache big enough to hold index n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();   /* memory failure */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        /* zero-fill the new extended part */
        if (new_cap > cache_cap)
            memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* fill in missing values, starting after the last computed one */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)18;
        } else if (i == 2) {
            cache[2] = (seq_t)100;
        } else if (i == 3) {
            cache[3] = (seq_t)570;
        } else if (i == 4) {
            cache[4] = (seq_t)3234;
        } else {
            /* recurrence: a(i) = 6*a(i-1) - 11*a(i-3) + 4*a(i-4) */
            seq_t part1 = ((seq_t)6) * cache[(size_t)(i - 1)];
            seq_t part2 = ((seq_t)4) * cache[(size_t)(i - 4)];
            seq_t sum   = part1 + part2;              /* no overflow for small n */
            seq_t part3 = ((seq_t)11) * cache[(size_t)(i - 3)];
            cache[(size_t)i] = sum - part3;          /* guaranteed non‑negative */
        }
    }
    computed_up_to = n;
    return cache[(size_t)n];
}

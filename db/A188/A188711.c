/*
 * A188711  Number of 7Xn binary arrays without the pattern 0 0 diagonally or
 *          vertically.
 * https://oeis.org/A188711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence (for n > 9): a(n) = 34*a(n-1) - 324*a(n-2) +
 *            1264*a(n-3) - 2236*a(n-4) + 1816*a(n-5) - 624*a(n-6) + 64*a(n-7)
 *            Initial values (for 1 ≤ n ≤ 9): a(1) = 34, a(2) = 697, a(3) =
 *            14925, a(4) = 318732, a(5) = 6814290, a(6) = 145764780, a(7) =
 *            3118943536, a(8) = 66743533552, a(9) = 1428321913952
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first listed term.  For n < 1 the value is undefined
       but we return 0 to be safe. */
    if (n < 1) {
        return 0;
    }

    /* On the first call we allocate a cache and populate the first nine
       terms.  The cache is 1‑based: cache[1]..cache[computed].
       We keep the cache as a static pointer so that subsequent calls reuse it. */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* highest index that has been computed   */
    static idx_t capacity = 0;   /* size of the allocated array (1‑based)   */

    if (cache == NULL) {
        /* Initial capacity large enough to store indices up to 9. */
        capacity = 10;               /* indices 0..9 ; 0 is unused */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return 0; /* allocation failure */

        /* Initialise first nine terms.  cache[0] is a dummy value. */
        static const seq_t init[10] = {
            0ULL,           /* unused */
            34ULL,
            697ULL,
            14925ULL,
            318732ULL,
            6814290ULL,
            145764780ULL,
            3118943536ULL,
            66743533552ULL,
            1428321913952ULL
        };
        for (idx_t i = 1; i <= 9; ++i) {
            cache[i] = init[i];
        }
        computed = 9;
    }

    /* If the requested index is less than or equal to what we already have,
       just return the cached value. */
    if (n <= computed) {
        return cache[n];
    }

    /* Ensure the cache is large enough to store up to n. */
    if ((idx_t)capacity <= n) {
        /* Grow to at least n+1 elements: 1‑based array. */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;
        capacity = (idx_t)new_cap;
    }

    /* Compute successive terms using the recurrence.  We use a
       decomposition into positive and negative parts to avoid
       intermediate unsigned underflow.  The recurrence guarantees
       that the positive part is greater than or equal to the
       negative part. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t pos = (seq_t)34 * cache[k-1]
                  + (seq_t)1264 * cache[k-3]
                  + (seq_t)1816 * cache[k-5]
                  + (seq_t)64  * cache[k-7];

        seq_t neg = (seq_t)324  * cache[k-2]
                  + (seq_t)2236 * cache[k-4]
                  + (seq_t)624  * cache[k-6];

        cache[k] = pos - neg;
    }

    computed = n;
    return cache[n];
}

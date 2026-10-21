/*
 * A211693  Number of -4..4 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having two, three or four distinct values for every i<=n and j<=n.
 * https://oeis.org/A211693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*a(n-1) - 630*a(n-2) + 4800*a(n-3) - 16675*a(n-4) * +
 *            13128*a(n-5) + 34290*a(n-6) + 17120*a(n-7) + 3176*a(n-8) * +
 *            192*a(n-9) * Initial values: a(1)=80, a(2)=712, a(3)=6328,
 *            a(4)=56184, * a(5)=498328, a(6)=4415576, a(7)=39087704, *
 *            a(8)=345688344, a(9)=3054441560
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Memoized computation of the sequence up to requested n. */
seq_t a(idx_t n) {
    /* Initial nine terms.  They are needed to seed the recurrence. */
    static const seq_t init[9] = {
        (seq_t)80ULL,
        (seq_t)712ULL,
        (seq_t)6328ULL,
        (seq_t)56184ULL,
        (seq_t)498328ULL,
        (seq_t)4415576ULL,
        (seq_t)39087704ULL,
        (seq_t)345688344ULL,
        (seq_t)3054441560ULL
    };

    /* Static cache of already computed terms.  The array is 0‑based
     * for convenience: cache[0] holds a(1).  Its size is stored in
     * cache_len. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Allocate or grow the cache to at least n terms. */
    if (n > cache_len) {
        if (cache_len == 0) {
            /* First allocation: create space for at least n terms. */
            cache = (seq_t *)malloc(sizeof(seq_t) * ((size_t)n));
            if (!cache) return (seq_t)0;  /* Allocation failure → zero. */
            cache_len = n;
        } else {
            /* Grow the cache. */
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * ((size_t)n));
            if (!new_cache) return (seq_t)0;  /* Allocation failure → zero. */
            cache = new_cache;
            /* Initialize newly allocated slots. */
            for (idx_t i = cache_len; i < n; ++i) cache[i] = (seq_t)0;
            cache_len = n;
        }

        /* Fill in the initial nine terms if they are not already present. */
        for (idx_t i = 0; i < 9 && i < n; ++i) {
            cache[i] = init[i];
        }

        /* Compute the remaining terms using the recurrence. */
        for (idx_t i = 9; i < n; ++i) {
            seq_t t1 = 40ULL   * cache[i - 1];
            seq_t t2 = 630ULL  * cache[i - 2];
            seq_t t3 = 4800ULL * cache[i - 3];
            seq_t t4 = 16675ULL* cache[i - 4];
            seq_t t5 = 13128ULL* cache[i - 5];
            seq_t t6 = 34290ULL* cache[i - 6];
            seq_t t7 = 17120ULL* cache[i - 7];
            seq_t t8 = 3176ULL * cache[i - 8];
            seq_t t9 = 192ULL  * cache[i - 9];
            cache[i] = t1 - t2 + t3 - t4 + t5 + t6 + t7 + t8 + t9;
        }
    }

    /* Return the requested term.  The function is 1‑based. */
    return cache[n - 1];
}

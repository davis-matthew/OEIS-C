/*
 * A232944  Number of n X 4 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, vertically or antidiagonally, and top
 *          left element zero.
 * https://oeis.org/A232944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A232944:
 * Number of n×4 arrays with entries in {0,1,2,3}, top‑left element 0, 
 * such that no element is horizontally, vertically, or antidiagonally
 * adjacent to its complement 3–x.  
 *
 * The values satisfy the empirical linear recurrence (for n > 11)
 *   a(n) = 29 a(n‑1) – 126 a(n‑2) + 186 a(n‑3) – 207 a(n‑4)
 *          + 40 a(n‑5) + 83 a(n‑6) – 29 a(n‑7) – 14 a(n‑8)
 *          + 8 a(n‑9) – a(n‑10)
 * The first eleven terms are stored explicitly:
 *   a(1)…a(11) = 27, 577, 13707, 328989, 7915555,
 *                190528543, 4586348177, 110402289283,
 *                2657600587981, 63973693477099,
 *                1539973156889093.
 */
#include <stdlib.h>

static const seq_t init_vals[11] = {
    (seq_t)27ULL,
    (seq_t)577ULL,
    (seq_t)13707ULL,
    (seq_t)328989ULL,
    (seq_t)7915555ULL,
    (seq_t)190528543ULL,
    (seq_t)4586348177ULL,
    (seq_t)110402289283ULL,
    (seq_t)2657600587981ULL,
    (seq_t)63973693477099ULL,
    (seq_t)1539973156889093ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* value not defined for index 0 */
    if (n <= 11) return init_vals[n-1];

    /* lazily allocate and fill a cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache_cap < n) {             /* need to extend the cache */
        size_t new_cap = n;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        }
        /* store the precomputed first eleven terms */
        for (idx_t i = 0; i < 11; ++i) {
            cache[i] = init_vals[i];
        }
        /* compute the remaining terms up to n */
        for (idx_t i = 11; i < n; ++i) {
            seq_t val = 29 * cache[i-1];
            val -= 126 * cache[i-2];
            val += 186 * cache[i-3];
            val -= 207 * cache[i-4];
            val += 40 * cache[i-5];
            val += 83 * cache[i-6];
            val -= 29 * cache[i-7];
            val -= 14 * cache[i-8];
            val += 8 * cache[i-9];
            val -= cache[i-10];
            cache[i] = val;
        }
        cache_cap = new_cap;
    }

    return cache[n-1];
}

/*
 * A208692  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A208692
 *  Recurrence: a(n) = a(n-1) + 26*a(n-2) + 6*a(n-3) - 78*a(n-4)
 *  Initial values: a(1)=21, a(2)=441, a(3)=1113, a(4)=11067
 *  (Valid for all n ≥ 1; behaviour for n < 1 does not matter.)
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset – return 0 */

    static seq_t *cache = NULL;   /* 1‑based cache: cache[0] = a(1) */
    static idx_t cap   = 0;        /* allocated entries */
    static idx_t size  = 0;        /* number of entries computed */

    /* initialise the cache on first call */
    if (cap == 0) {
        cap = 16;   /* start with a small block */
        cache = (seq_t*)malloc(cap * sizeof(*cache));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* grow the cache if enough space is not available for n */
    if (n > cap) {
        idx_t new_cap = cap << 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to index n */
    while (size < n) {
        idx_t idx = size + 1;   /* current 1‑based index to compute */
        seq_t val;

        if (idx == 1)          val = 21;
        else if (idx == 2)     val = 441;
        else if (idx == 3)     val = 1113;
        else if (idx == 4)     val = 11067;
        else {
            seq_t a1 = cache[size   - 1];   /* a(idx-1) */
            seq_t a2 = cache[size   - 2];   /* a(idx-2) */
            seq_t a3 = cache[size   - 3];   /* a(idx-3) */
            seq_t a4 = cache[size   - 4];   /* a(idx-4) */
            val = a1 + (seq_t)26 * a2 + (seq_t)6 * a3 - (seq_t)78 * a4;
        }

        cache[size++] = val;
    }

    return cache[n - 1];
}

/*
 * A079624  a(1) = a(2) = 1, a(3) = 6, a(n) = abs(a(n-1) - a(n-2) - a(n-3)).
 * https://oeis.org/A079624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = a(2) = 1, a(3) = 6,
 * a(n) = |a(n-1) - a(n-2) - a(n-3)|   for n ≥ 4
 */
#include <stdlib.h>
#include <string.h>

/* Cached values: 1‑based indexing, cache[0] is unused */
static seq_t *cache   = NULL;
static idx_t   capacity = 0;   /* allocated array size (in seq_t elements) */
static idx_t   size     = 0;   /* highest index computed so far */

/*
 * Compute a(n) with memoization.  Calls may arrive in arbitrary order.
 * The recurrence is evaluated exactly using unsigned arithmetic.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety; sequence is 1‑based */

    /* Ensure cache can hold indices up to n */
    if (n > size) {
        idx_t new_cap = capacity;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) return 0;  /* allocation failure – abort silently */

        /* copy existing data and free old buffer */
        if (cache) {
            memcpy(new_arr, cache, (size + 1) * sizeof(seq_t));  /* include unused 0 */
            free(cache);
        }
        cache = new_arr;
        capacity = new_cap;

        /* Compute missing terms up to n */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1 || i == 2) {
                val = 1;
            } else if (i == 3) {
                val = 6;
            } else {
                seq_t v1 = cache[i - 1];
                seq_t v2 = cache[i - 2];
                seq_t v3 = cache[i - 3];

                /* Compute |v1 - v2 - v3| without overflow */
                seq_t sum23 = v2 + v3;
                if (v1 >= sum23) {
                    val = v1 - sum23;
                } else {
                    val = sum23 - v1;
                }
            }
            cache[i] = val;
        }
        size = n;
    }
    return cache[n];
}

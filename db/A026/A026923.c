/*
 * A026923  Number of partitions of n into an odd number of parts, the
 *          greatest being 3; also, a(n+5) = number of partitions of n+2 into
 *          an even number of parts, each <= 3.
 * https://oeis.org/A026923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-6) - a(n-7) - a(n-10) +
 *            a(n-11) * for n > 11; Initial values: * a(1)=0, a(2)=0, a(3)=1,
 *            a(4)=0, a(5)=1, a(6)=1, a(7)=3, * a(8)=2, a(9)=4, a(10)=3,
 *            a(11)=6
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values, 1‑based */
static idx_t cached_size = 0;        /* highest index currently cached */

static const seq_t init_vals[12] = {
    0,  /* a(0) unused */
    0,  /* a(1) */
    0,  /* a(2) */
    1,  /* a(3) */
    0,  /* a(4) */
    1,  /* a(5) */
    1,  /* a(6) */
    3,  /* a(7) */
    2,  /* a(8) */
    4,  /* a(9) */
    3,  /* a(10) */
    6   /* a(11) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1; below offset, return 0 */

    if (n > cached_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                (size_t)(new_size + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Memory allocation failed – aborting is the safest choice. */
            abort();
        }
        cache = new_cache;

        /* Initialize base values up to 11 if needed. */
        if (cached_size < 11) {
            idx_t start = cached_size + 1;
            if (start < 1) start = 1;
            for (idx_t i = start; i <= 11 && i <= new_size; ++i) {
                cache[i] = init_vals[i];
            }
        }

        /* Compute values for all indices greater than the current cache. */
        idx_t first = cached_size + 1;
        if (first <= 11) first = 12;          /* recurrence starts at 12 */
        for (idx_t i = first; i <= new_size; ++i) {
            long long t = (long long)cache[i-1]   /* a(i-1) */
                        + (long long)cache[i-4]   /* a(i-4) */
                        - (long long)cache[i-5]   /* a(i-5) */
                        + (long long)cache[i-6]   /* a(i-6) */
                        - (long long)cache[i-7]   /* a(i-7) */
                        - (long long)cache[i-10]  /* a(i-10) */
                        + (long long)cache[i-11]; /* a(i-11) */
            cache[i] = (seq_t)t;
        }

        cached_size = new_size;
    }

    return cache[n];
}

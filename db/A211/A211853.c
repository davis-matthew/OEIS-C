/*
 * A211853  Number of nonnegative integer arrays of length 2n+8 with new
 *          values 0 upwards introduced in order, no three adjacent elements
 *          all unequal, and containing the value n+1.
 * https://oeis.org/A211853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) for n>7 * Initial values: *
 *            a(1)=1731, a(2)=6936, a(3)=21897, a(4)=57913, * a(5)=134164,
 *            a(6)=280751, a(7)=542235
 * Range:     u8 none, u16 n <= 4, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper macro to set the i-th element (1‑based) of the cache.  */
#define SET(i, v) cache[(i)] = (v)

/* Lookup table entry for 1..7.  */
static const seq_t init_vals[7] = {
    /* a(1) */ (seq_t)1731,
    /* a(2) */ (seq_t)6936,
    /* a(3) */ (seq_t)21897,
    /* a(4) */ (seq_t)57913,
    /* a(5) */ (seq_t)134164,
    /* a(6) */ (seq_t)280751,
    /* a(7) */ (seq_t)542235
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* 1‑based cache */
    static idx_t size = 0;          /* largest index cached */

    if (n == 0) { /* offset error: not defined; return 0 */
        return 0;
    }

    /* Grow the cache if necessary */
    if (n > size) {
        size_t new_size = (size_t)n + 1;      /* +1 so index n is valid */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; return 0 */
            return 0;
        }
        cache = tmp;
        /* Initialize newly allocated entries to 0 */
        for (idx_t i = size + 1; i <= n; ++i) {
            cache[i] = 0;
        }
        /* Fill initial values if not already present */
        for (idx_t i = 1; i <= 7 && i <= n; ++i) {
            SET(i, init_vals[i - 1]);
        }
        /* Compute remaining values using the recurrence */
        for (idx_t i = 8; i <= n; ++i) {
            seq_t val = 7 * cache[i - 1]
                        - 21 * cache[i - 2]
                        + 35 * cache[i - 3]
                        - 35 * cache[i - 4]
                        + 21 * cache[i - 5]
                        - 7  * cache[i - 6]
                        + cache[i - 7];
            SET(i, val);
        }
        size = n;
    } else if (size < 7) {
        /* If cache is smaller than 7, init the first 7 values once */
        size_t old_size = size;
        size = 7;
        seq_t *tmp = (seq_t *)realloc(cache, (size + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (idx_t i = old_size + 1; i <= size; ++i) {
            cache[i] = init_vals[i - 1];
        }
    }

    return cache[n];
}

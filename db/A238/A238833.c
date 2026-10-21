/*
 * A238833  a(n) = n-1 for n <= 2; thereafter a(n) = A238824(n-2) +
 *          A238832(n-1).
 * https://oeis.org/A238833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 for n <= 2; thereafter a(n) = A238824(n-2) +
 *            A238832(n-1). The sequence satisfies the linear recurrence a(n)
 *            = a(n-1) + 5 a(n-2) - a(n-3) - 7 a(n-4) - a(n-5) + 6 a(n-6) + 6
 *            a(n-7) + a(n-8) - a(n-9) for n >= 11, with initial values
 *            a(1)=0, a(2)=1, a(3)=0, a(4)=2, a(5)=2, a(6)=7, a(7)=16...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 53, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[11] = {
    0,          /* index 0 unused */
    0,          /* a(1) */
    1,          /* a(2) */
    0,          /* a(3) */
    2,          /* a(4) */
    2,          /* a(5) */
    7,          /* a(6) */
    16,         /* a(7) */
    40,         /* a(8) */
    101,        /* a(9) */
    246         /* a(10) */
};

static seq_t *cache   = NULL;   /* array storing computed terms (1‑based) */
static size_t cache_size = 0;    /* highest index that has been computed */
static size_t cache_cap  = 0;    /* allocated capacity of cache */

static void ensure_cache(idx_t n) {
    /* Grow the cache if necessary */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap == 0 ? 32 : cache_cap * 2;
        while (new_cap < (size_t)(n + 1)) {
            new_cap *= 2;
        }
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        /* Initialize new slots to 0 (security) */
        for (size_t i = cache_size + 1; i < new_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_cap = new_cap;
    }

    /* Initialise first ten terms if not done yet */
    if (cache_size < 10) {
        for (idx_t i = 1; i <= 10; ++i) {
            cache[i] = init_vals[i];
        }
        cache_size = 10;
    }

    /* Compute remaining terms up to n */
    if (cache_size < n) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t a1  = cache[i - 1];
            seq_t a2  = cache[i - 2];
            seq_t a3  = cache[i - 3];
            seq_t a4  = cache[i - 4];
            seq_t a5  = cache[i - 5];
            seq_t a6  = cache[i - 6];
            seq_t a7  = cache[i - 7];
            seq_t a8  = cache[i - 8];
            seq_t a9  = cache[i - 9];
            /* recurrence coefficients: 1, 5, -1, -7, -1, 6, 6, 1, -1 */
            cache[i] = a1
                     + 5 * a2
                     - a3
                     - 7 * a4
                     - a5
                     + 6 * a6
                     + 6 * a7
                     + a8
                     - a9;
        }
        cache_size = n;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of range, but safe */
    ensure_cache(n);
    return cache[n];
}

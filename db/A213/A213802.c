/*
 * A213802  Number of 4X4 0..n symmetric arrays with all rows summing to 2*n.
 * https://oeis.org/A213802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n−1) − 3*a(n−2) − 8*a(n−3) + 14*a(n−4) − 14*a(n−6) +
 *            8*a(n−7) + 3*a(n−8) − 4*a(n−9) + a(n−10) Initial values:
 *            a(1)=18, a(2)=169, a(3)=880, a(4)=3249, a(5)=9522, a(6)=23753,
 *            a(7)=52544, a(8)=106009, a(9)=198770, a(10)=351233 The
 *            implementation memoises computed values so that any call o...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 51, u64 n <= 173, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, calloc */

static seq_t *cache = NULL;      /* memoised values */
static idx_t cache_size = 0;     /* number of values stored (max index + 1) */
static idx_t cache_cap = 0;      /* current capacity of cache array */

/* Base values for n = 1 … 10 */
static const seq_t BASE[10] = {
    18,          /* a(1) */
    169,         /* a(2) */
    880,         /* a(3) */
    3249,        /* a(4) */
    9522,        /* a(5) */
    23753,       /* a(6) */
    52544,       /* a(7) */
    106009,      /* a(8) */
    198770,      /* a(9) */
    351233       /* a(10) */
};

/* Ensure that cache has room for at least N elements */
static void ensure_capacity(idx_t N) {
    if (cache_cap >= N) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 10;
    if (new_cap < N) new_cap = N;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – terminate program (no error handling needed) */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used by OEIS but safe */

    /* Reserve space */
    ensure_capacity(n);

    /* If n already computed, return it */
    if (cache_size >= n) {
        return cache[n - 1];
    }

    /* Fill in missing base values up to 10 if needed */
    if (cache_size < 10) {
        idx_t end = (n < 10) ? n : 10;
        for (idx_t i = cache_size + 1; i <= end; ++i) {
            cache[i - 1] = BASE[i - 1];
        }
        cache_size = end;
    }

    /* Compute values for n > 10 using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val =
            (seq_t)4 * cache[i - 2]                         /* 4*a(i-1) */
          - (seq_t)3 * cache[i - 3]                          /* -3*a(i-2) */
          - (seq_t)8 * cache[i - 4]                          /* -8*a(i-3) */
          + (seq_t)14 * cache[i - 5]                         /* +14*a(i-4) */
          - (seq_t)14 * cache[i - 7]                         /* -14*a(i-6) */
          + (seq_t)8 * cache[i - 8]                          /* +8*a(i-7) */
          + (seq_t)3 * cache[i - 9]                          /* +3*a(i-8) */
          - (seq_t)4 * cache[i - 10]                         /* -4*a(i-9) */
          + cache[i - 11];                                  /* +a(i-10) */
        cache[i - 1] = val;
    }
    cache_size = n;
    return cache[n - 1];
}

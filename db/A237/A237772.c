/*
 * A237772  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 188*a(n-4) + 512*a(n-6) + 336*a( n-8) - 64*a(n-10) for
 *            n>13 Initial values (n=1..13): 81, 300, 1176, 4480, 17768,
 *            67152, 263504, 1002432, 3967808, 15017792, 59024064, 224325120,
 *            886756160
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static const seq_t init_vals[13] = {
    81ULL, 300ULL, 1176ULL, 4480ULL, 17768ULL,
    67152ULL, 263504ULL, 1002432ULL, 3967808ULL,
    15017792ULL, 59024064ULL, 224325120ULL, 886756160ULL
};

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cache_size = 0;    /* number of terms stored */

static void ensure_cache_size(idx_t n) {
    if (n <= cache_size) return;
    size_t required = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!new_cache) abort();  /* allocation failed */
    /* Initialize new elements */
    for (size_t i = cache_size; i < required; ++i) {
        idx_t idx = (idx_t)i + 1;   /* 1‑based index */
        if (idx <= 13) {
            new_cache[i] = init_vals[idx - 1];
        } else {
            /* Compute using the recurrence */
            seq_t r1 = new_cache[idx - 5];   /* a(n-4) */
            seq_t r2 = new_cache[idx - 7];   /* a(n-6) */
            seq_t r3 = new_cache[idx - 9];   /* a(n-8) */
            seq_t r4 = new_cache[idx -11];   /* a(n-10) */
            new_cache[i] = r1 * 188ULL + r2 * 512ULL + r3 * 336ULL - r4 * 64ULL;
        }
    }
    cache = new_cache;
    cache_size = required;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined for n=0; return 0 as safety */
    ensure_cache_size(n);
    return cache[n - 1];
}

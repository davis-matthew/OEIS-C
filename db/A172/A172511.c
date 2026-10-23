/*
 * A172511  a(n) = a(n-1) * (11*a(n-1) - a(n-2)) / (a(n-1) + 4*a(n-2)), with
 *          a(0) = a(1) = 1.
 * https://oeis.org/A172511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * (11*a(n-1) - a(n-2)) / (a(n-1) + 4*a(n-2));
 *            a(0)=a(1)=1; Linear recurrence:
 *            a(n)=11*a(n-1)-33*a(n-2)+33*a(n-3)-11*a(n-4)+a(n-5); Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=35
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed values */
static size_t seq_cache_len = 0;  /* number of valid entries in cache */
static size_t seq_cache_cap = 0;  /* current allocation size */

/* Ensure the cache can hold at least `needed` entries. */
static void ensure_cache_capacity(size_t needed) {
    if (seq_cache_cap < needed) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Return cached value if already computed */
    if (n < seq_cache_len) return seq_cache[n];

    /* Initialize cache with the first five terms if not yet done */
    if (seq_cache == NULL) {
        ensure_cache_capacity(5);
        seq_cache_len = 5;
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)2;
        seq_cache[3] = (seq_t)7;
        seq_cache[4] = (seq_t)35;
    }

    /* Extend cache up to index n using the linear recurrence */
    for (size_t i = seq_cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = seq_cache[i-1] * (seq_t)11;
        seq_t t2 = seq_cache[i-2] * (seq_t)33;
        seq_t t3 = seq_cache[i-3] * (seq_t)33;
        seq_t t4 = seq_cache[i-4] * (seq_t)11;
        seq_t val = t1 - t2 + t3 - t4 + seq_cache[i-5];
        ensure_cache_capacity(i + 1);
        seq_cache[i] = val;
    }

    return seq_cache[n];
}

/*
 * A188551  Numbers located at angle turns in a pentagonal spiral.
 * https://oeis.org/A188551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-5) - 2*a(n-6) - a(n-10) + a(n-11) with
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=9, a(8)=11,
 *            a(9)=12, a(10)=14, a(11)=17
 * Range:     u8 n <= 52, u16 n <= 901, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cap   = 0;           /* allocated length of cache */
static size_t max_c = 0;           /* largest n for which a(n) is cached */

static const seq_t init[11] = {
    (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)5,
    (seq_t)7, (seq_t)9, (seq_t)11, (seq_t)12, (seq_t)14, (seq_t)17
};

static void ensure_capacity(size_t needed) {
    if (needed <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failed */
    if (!cache) memset(tmp, 0, new_cap * sizeof(seq_t));
    cache = tmp;
    cap   = new_cap;
}

/* Computes a(n) for any n >= 1, with memoisation. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined for n < 1 */
    /* Ensure cache can hold at least n elements (1‑based). */
    ensure_capacity((size_t)n);

    /* If we already know a(n), return it. */
    if ((size_t)n <= max_c) return cache[(size_t)n - 1];

    /* Fill initial values up to 11 if necessary. */
    if (max_c < 11) {
        size_t up = (size_t)(n < 11 ? n : 11);
        for (size_t i = max_c + 1; i <= up; ++i)
            cache[i - 1] = init[i - 1];
        max_c = up;
        if (n <= 11) return cache[(size_t)n - 1];
    }

    /* Now we must compute from max_c+1 up to n using the recurrence. */
    for (size_t i = max_c + 1; i <= (size_t)n; ++i) {
        /* Apply: a(i) = a(i-1) + 2*a(i-5) - 2*a(i-6) - a(i-10) + a(i-11). */
        seq_t val = cache[i - 2]                    /* a(i-1)   */
                  + 2 * cache[i - 6]                /* 2*a(i-5) */
                  - 2 * cache[i - 7]                /* -2*a(i-6)*/
                  - cache[i - 11]                   /* -a(i-10) */
                  + cache[i - 12];                  /* +a(i-11) */
        cache[i - 1] = val;
    }
    max_c = (size_t)n;
    return cache[(size_t)n - 1];
}

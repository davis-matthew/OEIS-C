/*
 * A159641  Positive numbers y such that y^2 is of the form x^2+(x+647)^2 with
 *          integer x.
 * https://oeis.org/A159641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; a(1)=613, a(2)=647,
 *            a(3)=685, a(4)=2993, a(5)=3235, a(6)=3497
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache for computed terms */
static size_t a_cache_len = 0;  /* number of terms stored in cache */
static size_t a_cache_cap = 0;  /* allocated capacity of cache */

/* compute a(n), n >= 1, using recurrence */
seq_t a(idx_t n) {
    /* offset is 1; behaviour for n < 1 is irrelevant */
    if (n < 1) return 0;

    idx_t idx = n - 1;   /* zero‑based index */

    /* initialise cache with first six terms on first call */
    if (!a_cache) {
        a_cache_cap = 16;
        a_cache_len = 6;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) return 0;  /* allocation failed */
        a_cache[0] = (seq_t)613;
        a_cache[1] = (seq_t)647;
        a_cache[2] = (seq_t)685;
        a_cache[3] = (seq_t)2993;
        a_cache[4] = (seq_t)3235;
        a_cache[5] = (seq_t)3497;
    }

    /* grow cache if necessary */
    if ((size_t)idx >= a_cache_cap) {
        size_t new_cap = a_cache_cap;
        while (new_cap <= (size_t)idx + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        for (size_t i = 0; i < a_cache_len; ++i) tmp[i] = a_cache[i];
        free(a_cache);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* generate missing terms up to required index */
    while (a_cache_len <= (size_t)idx) {
        seq_t v = (seq_t)6 * a_cache[a_cache_len - 3] - a_cache[a_cache_len - 6];
        a_cache[a_cache_len++] = v;
    }

    return a_cache[(size_t)idx];
}

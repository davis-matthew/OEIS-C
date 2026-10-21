/*
 * A209869  a(n) = a(a(n-1)) + a(n - a(n-2)) - a(a(n-3)) + a(a(n-5)), with
 *          a(1) to a(15) = 1.
 * https://oeis.org/A209869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-1)) + a(n-a(n-2)) - a(a(n-3)) + a(a(n-5)), a(n) = 1
 *            for n <= 15
 * Range:     u8 n <= 1744, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive formula with memoisation, computed bottom‑up for safety.
   The cache is expanded as needed; base values for 1…15 are set to 1. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 0‑based array, cache[0] unused */
static size_t cache_cap = 0;     /* number of slots currently allocated */

/* Ensure capacity for indices up to n (inclusive) and initialise
   the array if new memory is allocated. */
static void ensure_cap(idx_t n) {
    if (cache_cap <= n) {
        size_t old_cap = cache_cap;
        size_t new_cap = n + 1;                 /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                      /* allocation failure */

        /* initialise new slots */
        for (size_t i = old_cap; i < new_cap; ++i) tmp[i] = 0;

        /* set base values a(1)…a(15) = 1 if they fit */
        for (idx_t i = 1; i <= 15 && i < new_cap; ++i) tmp[i] = 1;

        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n <= 15) return 1;

    ensure_cap(n);

    /* iterate from the smallest required index up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 15) {          /* base case */
            cache[i] = 1;
            continue;
        }
        if (cache[i] != 0) continue;   /* already computed */

        /* All operands involve indices < i, so they are ready */
        idx_t idx1 = (idx_t)cache[i - 1];                    /* a(i-1) */
        idx_t idx2 = i - (idx_t)cache[i - 2];                /* i - a(i-2) */
        idx_t idx3 = (idx_t)cache[i - 3];                    /* a(i-3) */
        idx_t idx5 = (idx_t)cache[i - 5];                    /* a(i-5) */

        /* compute the recurrence */
        seq_t val = cache[idx1] + cache[idx2] - cache[idx3] + cache[idx5];
        cache[i] = val;
    }

    return cache[n];
}

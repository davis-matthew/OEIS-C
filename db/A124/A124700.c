/*
 * A124700  Number of base 7 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Number of base 7 circular n‑digit numbers with adjacent
 *            digits differing by at most 1. The sequence satisfies the linear
 *            recurrence (order 7) a(n) = 7 a(n‑1) – 15 a(n‑2) + 5 a(n‑3) + 15
 *            a(n‑4) – 9 a(n‑5) – 3 a(n‑6) + a(n‑7), for all n ≥ 8, with
 *            initial values below
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed values */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* First 8 values of the sequence */
static const seq_t init[8] = {
    (seq_t)1,   /* a(0) */
    (seq_t)7,   /* a(1) */
    (seq_t)19,  /* a(2) */
    (seq_t)43,  /* a(3) */
    (seq_t)111, /* a(4) */
    (seq_t)287, /* a(5) */
    (seq_t)763, /* a(6) */
    (seq_t)2051 /* a(7) */
};

/* Ensure that cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;                /* already available */

    idx_t new_size = n + (idx_t)1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                         /* allocation failure */
    cache = tmp;

    /* Fill any new slots */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];
            cache[i] = 7*a1 - 15*a2 + 5*a3 + 15*a4 - 9*a5 - 3*a6 + a7;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}

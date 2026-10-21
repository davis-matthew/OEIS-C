/*
 * A226694  Pell equation solutions (32*a(n))^2 - 41*(5*b(n))^2 = -1 with b(n)
 *          := A226695(n), n>=0.
 * https://oeis.org/A226694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A226694 – Pell equation solutions.
 * Formula: a(n) = 4098*a(n‑1) – a(n‑2),  a(0) = 1,  a(1) = 4099
 * Offset: 0
 */
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;      /* dynamic array storing computed terms */
static size_t cache_cap = 0;     /* allocated capacity of the array */
static idx_t  max_computed = 0;  /* largest index that has been computed */

/* Ensure that 'cache' can hold terms up to index 'n' (inclusive) */
static void ensure_cache(idx_t n) {
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? (size_t)(n + 1) : cache_cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence A226694 */
seq_t a(idx_t n) {
    /* Prepare storage */
    ensure_cache(n);

    /* Initialise base cases if not done yet */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)4099;
        max_computed = (n >= 1) ? 1 : 0;
    }

    /* Compute subsequent terms up to n */
    while (max_computed < n) {
        seq_t next = (seq_t)4098 * cache[max_computed] - cache[max_computed - 1];
        cache[++max_computed] = next;
    }

    return cache[n];
}

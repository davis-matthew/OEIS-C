/*
 * A031985  Numbers with exactly five distinct base-8 digits.
 * https://oeis.org/A031985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_five_distinct_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer whose base‑8 representation contains exactly five distinct digits, in increasing order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_five_distinct_base8.h"

/* Ensure capacity for the static cache */
static void ensure_capacity(size_t required, size_t *cap, seq_t **arr) {
    if (*cap >= required) return;
    size_t newcap = (*cap) ? (*cap) * 2 : 16;
    if (newcap < required) newcap = required;
    seq_t *newp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *arr = newp;
    *cap = newcap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Static cache and state */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of terms stored */
    static size_t cap  = 0;      /* capacity of cache array */
    static seq_t last_val = 0;   /* last candidate examined */

    if (n == 0) return 0;   /* undefined but guard */

    /* Allocate cache on first call */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        size = 0;
        last_val = 0;
    }

    /* Generate terms until we have at least n terms */
    seq_t candidate = last_val + 1;
    while (size < (size_t)n) {
        if (has_five_distinct_base8(candidate)) {
            ensure_capacity(size + 1, &cap, &cache);
            cache[size++] = candidate;
        }
        ++candidate;
    }
    last_val = candidate - 1;

    return cache[n - 1];
}

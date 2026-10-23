/*
 * A032969  Numbers with the property that all pairs of consecutive base-6
 *          digits differ by more than 1.
 * https://oeis.org/A032969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032969: numbers whose consecutive base‑6 digits differ by more than 1.
 * The function scans natural numbers, testing each one for the property, and caches
 * the accepted numbers so that subsequent calls are O(1). */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff all pairs of consecutive base‑6 digits of x differ by > 1. */
static bool is_valid(seq_t x)
{
    if (x == 0)            /* 0 is not a positive integer in the sequence */
        return false;
    seq_t prev = x % 6;    /* least‑significant digit */
    x /= 6;
    while (x > 0) {
        seq_t d = x % 6;
        /* Compute |prev - d| and check that it is > 1. */
        if (((prev >= d) ? (prev - d) : (d - prev)) <= 1)
            return false;
        prev = d;
        x /= 6;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static cache of already computed terms (1‑based indexing). */
    static seq_t *cache      = NULL;
    static size_t capacity   = 0;
    static size_t filled     = 0;      /* number of terms in cache */
    static seq_t next_candidate = 1;   /* next natural number to test */

    /* Grow the cache if we haven't reached the required index. */
    if (filled < (size_t)n) {
        if (cache == NULL) {
            capacity = 16;
            cache = (seq_t *)malloc(capacity * sizeof(seq_t));
            if (!cache) exit(1);
        }
        while (filled < (size_t)n) {
            /* Find the next valid number. */
            while (!is_valid(next_candidate))
                ++next_candidate;

            /* Store it in the cache. */
            if (filled + 1 >= capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                capacity = new_cap;
            }
            cache[filled + 1] = next_candidate;  /* keep cache[0] unused for 1‑based indexing */
            ++filled;
            ++next_candidate;
        }
    }
    return cache[(size_t)n];
}

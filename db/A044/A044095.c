/*
 * A044095  Numbers k such that string 3,3 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2357, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Search: numbers k such that the base‑5 representation contains the
   substring 33, but k−1 does not contain it. */

/* test whether u contains \"33\" in base‑5 */
static bool contains33(seq_t u) {
    if (u == 0) return false;
    /* 128 digits comfortably exceed the width of any seq_t up to
       128‑bit (≈55 base‑5 digits). */
    int digits[128];
    int len = 0;
    while (u > 0) {
        digits[len++] = (int)(u % 5);
        u /= 5;
    }
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 3 && digits[i - 1] == 3)
            return true;
    }
    return false;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static seq_t next_candidate = 1;

/* Return the n‑th term of OEIS sequence A044095.
   n is 1‑based; a(1) is the first listed term (18). */
seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for offset < 1 */

    while (cache_size < n) {
        /* Find next number that satisfies the condition */
        while (true) {
            if (contains33(next_candidate) && !contains33(next_candidate - 1))
                break;
            ++next_candidate;
        }

        /* Grow cache if necessary */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store found number and advance the candidate for next search */
        cache[cache_size++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}

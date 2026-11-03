/*
 * A067635  a(1) = 1; set of digits of a(n)^2 is a proper subset of the set of
 *          digits of a(n+1)^2.
 * https://oeis.org/A067635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067635: a(1) = 1; for n > 1,
   a(n) is the smallest integer > a(n-1) such that the set of decimal
   digits of a(n-1)^2 is a proper subset of the set of decimal digits
   of a(n)^2. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the digit presence array for a number x.
   present[d] is true if digit d occurs in x. */
static void digit_present(seq_t x, bool present[10]) {
    for (int d = 0; d < 10; ++d) present[d] = false;
    if (x == 0) { present[0] = true; return; }
    while (x != 0) {
        int d = (int)(x % 10);
        present[d] = true;
        x /= 10;
    }
}

/* Return true if setPrev is a proper subset of setCand. */
static bool proper_subset(const bool setPrev[10], const bool setCand[10]) {
    bool strict = false;
    for (int d = 0; d < 10; ++d) {
        if (setPrev[d] && !setCand[d]) return false;   /* missing digit */
        if (!setPrev[d] && setCand[d]) strict = true;  /* new digit added */
    }
    return strict;
}

/* Compute a(n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but safe */

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t size = 0;

    /* Initialise cache with a(1) = 1 if it does not exist yet */
    if (capacity == 0) {
        capacity = 4;
        cache = mem_new(seq_t, capacity);
        cache[0] = (seq_t)1;   /* a(1) */
        size = 1;
    }

    /* Generate terms until we have enough */
    while (size < n) {
        seq_t prev = cache[size - 1];

        bool prevPresent[10];
        digit_present(prev * prev, prevPresent);

        seq_t candidate = prev + (seq_t)1;
        while (true) {
            seq_t sqCand = candidate * candidate;
            bool candPresent[10];
            digit_present(sqCand, candPresent);
            if (proper_subset(prevPresent, candPresent)) {
                /* Append candidate */
                if (size >= capacity) {
                    capacity *= 2;
                    cache = mem_resize(seq_t, cache, capacity);
                }
                cache[size] = candidate;
                ++size;
                break;
            }
            ++candidate;
        }
    }

    return cache[n - 1];
}

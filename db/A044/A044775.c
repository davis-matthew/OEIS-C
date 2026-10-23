/*
 * A044775  Numbers n such that string 6,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044775: the nth positive integer whose decimal representation
 * contains the substring "62" but n+1 does not.  Brute‑force scanning with
 * memoization. */
#include <stdlib.h>
#include <stdbool.h>

static bool contains_62(seq_t x) {
    seq_t prev = 0;
    while (x) {
        seq_t curr = x % 10;
        if (prev == 2 && curr == 6) return true;
        prev = curr;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t used  = 0;
    static seq_t last_checked = 1;

    /* Ensure cache has been allocated once */
    if (!cache) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) { /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
    }

    /* Extend cache until we have at least n entries */
    while (used < n) {
        last_checked++;

        if (contains_62(last_checked) && !contains_62(last_checked + 1)) {
            /* Grow cache if necessary */
            if (used == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failed */
                cache = tmp;
                cap = newcap;
            }
            cache[used++] = last_checked;
        }
    }

    return cache[n - 1];
}

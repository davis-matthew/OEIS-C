/*
 * A192689  Squares for which no final group of decimal digits less than the
 *          total forms a square.
 * https://oeis.org/A192689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 3685, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192689: squares for which no proper final suffix of the
   decimal representation is itself a perfect square. */

/* The function returns the n‑th term (n ≥ 1).  It is purely deterministic
   and memoises all computed values, so subsequent calls faster. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t) */
#include "../utils/grow_seq.h"    /* void grow_seq(seq_t **arr, size_t *cap, size_t need) */

/* Append a new term to the dynamic cache.  grow_seq handles construction
   of seq_t elements if necessary. */
static void append_term(seq_t val, seq_t **cache, size_t *len, size_t *cap) {
    if (*len == *cap) {
        grow_seq(cache, cap, *cap + 1);
    }
    (*cache)[(*len)++] = val;
}

/* Test whether x is a perfect square, using the provided floor_sqrt.
   For arbitrary‑precision types, multiplication and comparison are
   overloaded. */
static bool is_square(seq_t x) {
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Determine whether a square value `sq` satisfies the sequence condition.
   It must be a square itself and no proper final suffix (in decimal)
   may also be a square.  The function rejects single‑digit squares
   (0,1,4,9). */
static bool good_square(seq_t sq) {
    /* Exclude trivial one‑digit squares 0,1,4,9. */
    if (sq < 10) return false;

    /* Find number of decimal digits. */
    seq_t pow10 = 1;
    unsigned digits = 0;
    while (pow10 <= sq) {
        pow10 *= 10;
        ++digits;
    }

    /* Check every proper suffix (length 1 … digits‑1). */
    seq_t mod = 10;   /* 10^1 */
    for (unsigned k = 1; k < digits; ++k) {
        seq_t suffix = sq % mod;
        if (is_square(suffix))
            return false;   /* suffix is a square → reject */
        mod *= 10;          /* next power of 10 */
    }
    return true;   /* no suffix is a square */
}

/* Main function: return the n‑th member of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for n=0, safe return */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_i = 1;   /* integer whose square to test next */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t sq = (seq_t)next_i * (seq_t)next_i;
        if (good_square(sq))
            append_term(sq, &cache, &cache_len, &cache_cap);
        ++next_i;
    }

    return cache[n - 1];
}

/*
 * A032866  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A032866
   Numbers whose base‑3 representation digits d(m)…d(0) satisfy
   d(m) ≥ d(m‑1) ≤ d(m‑2) ≥ d(m‑3) ≤ … (alternating inequalities). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether v satisfies the alternating inequality condition. */
static inline bool _is_valid(seq_t v) {
    if (v == 0) return false;                /* 0 is outside the offset (a(1)=1). */

    /* Store base‑3 digits: digits[0] = least significant. */
    unsigned char digits[256];
    size_t len = 0;
    while (v > 0) {
        digits[len++] = (unsigned char)(v % 3);
        v = v / 3;
    }

    /* A single‑digit number is always valid. */
    if (len == 1) return true;

    /* Traverse from most significant to least. */
    size_t idx = len - 1;              /* index of current (higher) digit. */
    bool must_ge = true;               /* first comparison is “≥”. */

    while (idx > 0) {
        unsigned char a = digits[idx];     /* higher digit  */
        unsigned char b = digits[idx - 1]; /* lower digit   */

        if (must_ge) {
            if (a < b) return false;      /* need a ≥ b */
        } else {
            if (a > b) return false;      /* need a ≤ b */
        }
        must_ge = !must_ge;               /* flip direction */
        --idx;
    }
    return true;
}

/* Return the n‑th term (a(1)=1). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* out of range, not used by tests */

    /* Static cache to memoise previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Extend cache until it contains at least n terms. */
    if (cache_len < (size_t)n) {
        /* Ensure sufficient capacity. */
        if (cache_cap < (size_t)n) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            while (newcap < (size_t)n) newcap *= 2;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache_cap = newcap;
        }

        /* Start search from the first integer if cache empty,
           otherwise from the next integer after the last cached value. */
        seq_t x = (cache_len == 0) ? 1 : (cache[cache_len - 1] + 1);

        /* Find missing terms. */
        while (cache_len < (size_t)n) {
            if (_is_valid(x)) {
                cache[cache_len++] = x;
            }
            x = x + 1;
        }
    }

    return cache[n - 1];
}

/*
 * A039294  Numbers whose base-6 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th natural number whose base‑6 representation
 *            contains an equal non‑zero number of digits 0 and 2
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether a number has the required property. */
static inline bool is_valid(seq_t val) {
    seq_t temp = val;
    seq_t cnt0 = 0, cnt2 = 0;
    while (temp > 0) {
        seq_t rem = temp % 6;
        if (rem == 0)  cnt0++;
        else if (rem == 2)  cnt2++;
        temp /= 6;
    }
    return (cnt0 > 0) && (cnt0 == cnt2);
}

/* Static cache for already computed terms. */
static seq_t *cache      = NULL;
static size_t capacity   = 0;
static idx_t  cache_len  = 0;   /* number of terms cached */
static idx_t  next_start = 1;   /* next integer to test */

/* Return the n-th term of A039294 (1‑based index). */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* initialize cache on first call */
    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t val = (seq_t)next_start;
        if (is_valid(val)) {
            if (cache_len >= capacity) {
                capacity <<= 1;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
        next_start++;
    }

    return cache[n - 1];
}

/*
 * A228224  Number of second differences of arrays of length 9 of numbers in
 *          0..n.
 * https://oeis.org/A228224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120*n^7 - 42*n^6 - 1158*n^5 + 6945*n^4 * - 13980*n^3 +
 *            13512*n^2 - 4887*n + 1 * (Horner's rule is used for the small
 *            integer case. * For arbitrary‑precision seq_t a recurrence with
 *            memoisation * is used.)
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[8] = {
    511ULL, 19171ULL, 242461ULL, 1688101ULL,
    8006491ULL, 29066311ULL, 86929081ULL, 224817481ULL
};

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n < 1, safe fallback */

    /* Small sequence types (≤ 64 bit): use the polynomial directly */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 r = 120;
        r = r * n - 42;
        r = r * n - 1158;
        r = r * n + 6945;
        r = r * n - 13980;
        r = r * n + 13512;
        r = r * n - 4887;
        r = r * n + 1;
        return (seq_t)r;   /* truncates correctly when the result fits */
    }

    /* Arbitrary‑precision seq_t: memoised recurrence */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    if (n > cached_len) {
        /* grow the cache to size n */
        seq_t *new_cache = cached_len == 0
            ? (seq_t *)malloc(n * sizeof(seq_t))
            : (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed */
        cache = new_cache;
        while (cached_len < n) {
            if (cached_len < 8) {
                cache[cached_len] = init_terms[cached_len];
            } else {
                cache[cached_len] =
                    8 * cache[cached_len - 1]
                  - 28 * cache[cached_len - 2]
                  + 56 * cache[cached_len - 3]
                  - 70 * cache[cached_len - 4]
                  + 56 * cache[cached_len - 5]
                  - 28 * cache[cached_len - 6]
                  + 8 * cache[cached_len - 7]
                  - cache[cached_len - 8];
            }
            ++cached_len;
        }
    }

    return cache[n - 1];
}

/*
 * A039297  Numbers whose base-6 representation has the same nonzero number of
 *          0's and 5's.
 * https://oeis.org/A039297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th positive integer whose base‑6 representation has an
 *            equal positive number of digit 0 and digit 5. The implementation
 *            searches for the terms incrementally and caches the results so
 *            that repeated calls are efficient
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;           /* next candidate to test */

    if (n == 0) return 0;            /* undefined for n=0, guard against it */

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n terms */
    while (cache_len < n) {
        seq_t cand = next;
        seq_t tmp  = cand;
        int c0 = 0, c5 = 0;

        while (tmp > 0) {
            seq_t digit = tmp % 6;
            if (digit == 0)   c0++;
            else if (digit == 5) c5++;
            tmp /= 6;
        }

        if (c0 == c5 && c0 > 0) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        ++next;
    }

    return cache[n - 1];
}

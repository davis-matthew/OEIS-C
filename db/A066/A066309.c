/*
 * A066309  Numbers k such that k > (product of digits of k) * (sum of digits
 *          of k).
 * https://oeis.org/A066309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066309: numbers k such that k > (product of digits of k) * (sum of digits of k) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ok.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* maximum number of terms we cache (sufficient for n up to ~10000) */
    enum { MAX_CACHE = 50000 };
    static seq_t cache[MAX_CACHE];
    static size_t sz   = 0;   /* number of cached terms */
    static seq_t last = 0;   /* last integer examined */

    if (n <= 0) return (seq_t)0;   /* undefined for n = 0, but return 0 */

    /* Ensure we have at least n terms cached */
    while (sz < (size_t)n) {
        seq_t cand = ++last;
        if (ok(cand)) {
            if (sz < MAX_CACHE) {
                cache[sz] = cand;
            }
            sz++;
        }
    }

    if (n <= MAX_CACHE) {
        return cache[n - 1];
    } else {
        /* n is larger than the cache size – compute the term without caching */
        size_t count = sz;
        while (count < (size_t)n) {
            seq_t cand = ++last;
            if (ok(cand)) {
                count++;
            }
        }
        return last;
    }
}

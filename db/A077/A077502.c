/*
 * A077502  Smallest square greater than the previous term and beginning with
 *          n.
 * https://oeis.org/A077502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest square > a(n‑1) whose decimal representation
 *            begins with the decimal digits of n. a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Ensure the cache holds indices up to n. */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap) {
    if (n < (idx_t)*cap) return;
    size_t newcap = *cap ? *cap : 1;
    while (newcap <= (size_t)n) newcap <<= 1;
    *cache = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
    *cap   = newcap;
}

/* Number of decimal digits of an unsigned integer. */
static size_t digits_idx(idx_t x) {
    size_t d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* Compute 10^e as seq_t.  e is small (at most a few dozen) so a loop is fine. */
static seq_t pow10_seq(idx_t e) {
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 0;

    if (!cache) {                     /* initialise a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        cap   = 1;
        cache[0] = (seq_t)0;
        last   = 0;
    }

    if (n > last) {                   /* compute all missing values */
        ensure_cache(n, &cache, &cap);
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t prev = (i == 0) ? (seq_t)0 : cache[i - 1];
            seq_t result = 0;

            /* search increasing number of trailing digits */
            for (idx_t t = 0; ; ++t) {
                seq_t pow10 = pow10_seq(t);
                seq_t low  = (seq_t)i * pow10;
                seq_t high = ((seq_t)i + 1) * pow10 - 1;
                seq_t start = prev + 1;
                if (start < low) start = low;

                /* ceil(sqrt(start)) */
                seq_t k = floor_sqrt(start);
                if (k * k < start) ++k;

                seq_t kmax = floor_sqrt(high);
                if (k <= kmax) {              /* found suitable square */
                    result = k * k;
                    break;
                }
            }
            cache[i] = result;
        }
        last = n;
    }

    return cache[n];
}

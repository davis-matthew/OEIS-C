/*
 * A145114  Numbers of length n binary words with fewer than 6 0-digits
 *          between any pair of consecutive 1-digits.
 * https://oeis.org/A145114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A145114: Number of length‑n binary words with fewer than 6 zeros between any
 * pair of consecutive 1‑digits.
 *
 * Recurrence (valid for n ≥ 8):
 *     a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32, a(6)=64, a(7)=128
 *     a(n)=3·a(n−1) − 2·a(n−2) − a(n−7) + a(n−8)
 *
 * The function memoises computed values in a static array that grows on
 * demand, so it works for any call order and for any seq_t/idx_t
 * (unsigned integer or arbitrary‑precision integer). */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;          /* allocated capacity */
    static idx_t    done  = (idx_t)-1;   /* highest index computed (-1 means none) */

    /* initialise cache with the first eight terms (if not already done) */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)32;
        cache[6] = (seq_t)64;
        cache[7] = (seq_t)128;
        done = 7;
    }

    /* if the desired index is already cached, return it */
    if (n <= done) return cache[n];

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
        if (!tmp) return cache[n]; /* in case realloc fails, return last known value */
        cache = tmp;
        cap   = (size_t)(n + 1);
    }

    /* compute missing terms up to n */
    idx_t start = (done < 7) ? (idx_t)8 : done + 1; /* start from 8 if we haven't reached it yet */
    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t7 = cache[i - 7];
        seq_t t8 = cache[i - 8];
        cache[i] = (seq_t)3 * t1 - (seq_t)2 * t2 - t7 + t8;
    }
    done = n;
    return cache[n];
}

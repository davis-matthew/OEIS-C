/*
 * A131940  Least common multiple of {1, 7, 13, 19, 25, ..., (6n+1)}
 *          (A016921).
 * https://oeis.org/A131940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm_{k=0..n} (6k+1) (with a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;   /* largest index currently cached (inclusive) */

    if (cache == NULL) {
        /* Allocate for n = 0 */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed, return 0 as fail‑safe */
        cache[0] = 1;                      /* a(0) = 1 */
        cached_upto = 0;
    }

    if (n > cached_upto) {
        /* Extend cache to size n+1 (indices 0..n) */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;                  /* allocation failed */
        cache = tmp;

        seq_t prev = cache[cached_upto];     /* a(cached_upto) */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            seq_t term = (seq_t)6 * i + 1;   /* 6i + 1 */
            seq_t g = gcd(prev, term);      /* gcd(a(i-1), term) */
            seq_t l = (prev / g) * term;    /* lcm(prev, term) */
            cache[i] = l;
            prev = l;
        }
        cached_upto = n;
    }

    return cache[n];
}

/*
 * A152134  Maximal length of rook tour on an n X n+3 board.
 * https://oeis.org/A152134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) * +
 *            3*a(n-6) - a(n-7) for n > 7 * (Base values for 1 ≤ n ≤ 7 are
 *            hard‑coded numerically.)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mul_small.h"   /* provides seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* largest n for which cache[n] is valid */

    if (n == 0)                 /* out of range – not used by callers */
        return 0;

    /* Resize cache if we need values beyond what we have already computed */
    if (n > max_computed) {
        idx_t new_size = n + 1;          /* allocate index 0..n (index 0 unused) */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (new_cache == NULL)
            abort();                    /* caller cannot recover */

        /* copy previously computed values, if any */
        if (cache != NULL) {
            memcpy(new_cache, cache, sizeof(seq_t) * (max_computed + 1));
            free(cache);
        }
        cache = new_cache;

        /* initialise base values 1 … 7 (they are the first 7 terms of the sequence) */
        if (new_size >  1) cache[1] = 8;
        if (new_size >  2) cache[2] = 24;
        if (new_size >  3) cache[3] = 54;
        if (new_size >  4) cache[4] = 102;
        if (new_size >  5) cache[5] = 174;
        if (new_size >  6) cache[6] = 270;
        if (new_size >  7) cache[7] = 396;

        /* compute any missing terms using the recurrence; from i=8 upwards */
        idx_t start = max_computed + 1;
        if (start < 8) start = 8;
        for (idx_t i = start; i <= n; ++i) {
            seq_t t1 = mul_small(cache[i-1], 3);   /* 3*a(i-1) */
            seq_t t2 = mul_small(cache[i-2], 3);   /* 3*a(i-2) */
            seq_t t3 = mul_small(cache[i-5], 3);   /* 3*a(i-5) */
            seq_t t4 = mul_small(cache[i-6], 3);   /* 3*a(i-6) */

            /* a(i) = t1 - t2 + a(i-3) + a(i-4) - t3 + t4 - a(i-7) */
            seq_t result = t1 - t2 + cache[i-3] + cache[i-4]
                           - t3 + t4 - cache[i-7];
            cache[i] = result;
        }

        max_computed = n;
    }

    return cache[n];
}

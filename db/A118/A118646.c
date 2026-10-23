/*
 * A118646  a(n) is the number of binary strings of length n such that there
 *          exists a subsequence of length 4 containing 3 or more ones.
 * https://oeis.org/A118646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-6) + 13·2^(n-6)
 * initial values: a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=13, a(6)=31
 */
#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static idx_t   max_comp   = 0;

    /* base values for indices 1..6 */
    static const seq_t base_vals[7] = {0, 0, 0, 1, 5, 13, 31};

    /* guard against calls with n < 1 (offset is 1) */
    if (n < 1) return (seq_t)0;

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 7;                     /* enough for indices 0..6 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();              /* unrecoverable failure */
        /* fill base values */
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = base_vals[i];
        cache[0] = (seq_t)0;              /* dummy entry */
        max_comp = 6;
    }

    /* enlarge cache to accommodate up to n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms if any */
    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t value;
            if (i <= 6) {
                value = base_vals[i];
            } else {
                /* recurrence part */
                value = cache[i-1] + cache[i-2] + cache[i-4]
                        - cache[i-6];
                /* 2^(i-6) using big_pow for arbitrary‑precision */
                seq_t pow2 = big_pow((seq_t)2, (seq_t)(i-6));
                value = value + (seq_t)13 * pow2;
            }
            cache[i] = value;
        }
        max_comp = n;
    }

    return cache[n];
}

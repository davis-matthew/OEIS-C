/*
 * A189445  Number of n X 5 array permutations with each element moving zero
 *          or one space horizontally or diagonally.
 * https://oeis.org/A189445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 19*a(n-2) - 11*a(n-3) + 36*a(n-4) + 3*a(n-5)
 *            (n > 6). Initial values for 1 ≤ n ≤ 6 are: a(1)=8, a(2)=225,
 *            a(3)=5080, a(4)=118128, a(5)=2735828, a(6)=63367633
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for 1 … 6 */
    static const seq_t base[6] = {
        (seq_t)8,
        (seq_t)225,
        (seq_t)5080,
        (seq_t)118128,
        (seq_t)2735828,
        (seq_t)63367633
    };

    if (n <= 6)
        return base[(size_t)(n - 1)];

    /* Static cache for memoisation (1‑based, cache[0] unused) */
    static seq_t *cache   = NULL;
    static size_t cache_len    = 0;   /* allocated length (max index + 1)   */
    static idx_t  computed    = 0;    /* highest index that has been computed */

    /* Allocate or grow the cache as required */
    size_t needed = (size_t)n + 1;    /* to hold indices 0 … n */
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) exit(1);
        cache_len = needed;
    } else if (cache_len < needed) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_len = needed;
    }

    /* Initialise the first six positions once */
    if (computed < 6) {
        for (size_t i = (size_t)(computed + 1); i <= 6; ++i)
            cache[i] = base[i - 1];
        computed = 6;
    }

    /* Compute up to n if necessary */
    if (n > computed) {
        for (size_t i = (size_t)(computed + 1); i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];

            cache[i] = (seq_t)24 * a1
                     - (seq_t)19 * a2
                     - (seq_t)11 * a3
                     + (seq_t)36 * a4
                     + (seq_t)3  * a5;
        }
        computed = n;
    }

    return cache[n];
}

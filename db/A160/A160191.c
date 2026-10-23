/*
 * A160191  Lodumo_8 of Lucas numbers.
 * https://oeis.org/A160191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-6) - a(n-12), n ≥ 12; Initial values: a(0)=2,
 *            a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=11, a(6)=10, a(7)=5,
 *            a(8)=15, a(9)=12, a(10)=19, a(11)=23
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* stored initial terms */
    static const seq_t base[12] = {
        (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)4,
        (seq_t)7, (seq_t)11, (seq_t)10, (seq_t)5,
        (seq_t)15, (seq_t)12, (seq_t)19, (seq_t)23
    };

    /* Return pre‑computed term for n < 12 */
    if (n < 12) {
        return base[(idx_t)n];
    }

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    /* Allocate or extend cache to accommodate up to n */
    if (cache_len <= n) {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        /* Initialize base values if this is the first allocation */
        if (!cache || cache_len == n + 1) {
            for (idx_t i = 0; i < 12; ++i) {
                cache[i] = base[i];
            }
        }
    }

    /* Compute values from 12 up to n using the recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        cache[i] = ((seq_t)2 * cache[i - 6]) - cache[i - 12];
    }

    return cache[n];
}

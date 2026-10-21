/*
 * A233492  Number of (n+2) X (3+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally or antidiagonally downwards.
 * https://oeis.org/A233492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 191*a(n-1)+134*a(n-2)-38637*a(n-3)+3177*a(n-4) *
 *            +1502613*a(n-5)-3705102*a(n-6)+2380185*a(n-7) * for n ≥ 8 *
 *            Initial values: * a(1) = 8946936 * a(2) = 1709348346 * a(3) =
 *            325881917874 * a(4) = 62127117326151 * a(5) = 11844001767009636
 *            * a(6) = 2257956973020836034 * a(7) = 43046004342945997...
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache indexed from 1 */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_max = 0;

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        /* initialise newly added elements to zero */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first seven terms once */
    static bool init_done = false;
    if (!init_done) {
        /* cache[0] is unused, set to zero */
        cache[0] = 0;
        cache[1] = seq_from_str("8946936");
        cache[2] = seq_from_str("1709348346");
        cache[3] = seq_from_str("325881917874");
        cache[4] = seq_from_str("62127117326151");
        cache[5] = seq_from_str("11844001767009636");
        cache[6] = seq_from_str("2257956973020836034");
        cache[7] = seq_from_str("430460043429459971151");
        init_done = true;
    }

    /* compute further terms if required */
    idx_t start = computed_max + 1;
    if (start < 8) start = 8;
    for (idx_t i = start; i <= n; ++i) {
        seq_t sPos = (seq_t)191   * cache[i-1] +
                     (seq_t)134   * cache[i-2] +
                     (seq_t)3177  * cache[i-4] +
                     (seq_t)1502613 * cache[i-5] +
                     (seq_t)2380185 * cache[i-7];
        seq_t sNeg = (seq_t)38637 * cache[i-3] +
                     (seq_t)3705102 * cache[i-6];
        cache[i] = sPos - sNeg;
    }
    computed_max = n;

    return cache[n];
}

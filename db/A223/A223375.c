/*
 * A223375  3X3 square grid graph coloring a rectangular array: number of nX4
 *          0..8 arrays where 0..8 label nodes of the square grid graph and
 *          every array movement to a horizontal or vertical neighbor moves
 *          along an edge of this graph.
 * https://oeis.org/A223375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) - 319*a(n-2) - 105*a(n-3) + 4371*a(n-4) -
 *            4177*a(n-5) - 9101*a(n-6) + 12732*a(n-7) - 5244*a(n-8) +
 *            704*a(n-9) a(1) = 192, a(2) = 5808, a(3) = 192752, a(4) =
 *            6443888, a(5) = 215773456, a(6) = 7226963248, a(7) =
 *            242067684912, a(8) = 8108163611488, a(9) = 271587161033712
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms – 1-based indexing */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size of cache (indices 0..cap-1) */
    static size_t len = 0;   /* highest index with computed value */

    /* first 9 terms – hardcoded initial values from the OEIS entry */
    static const seq_t init[9] = {
        (seq_t)192ULL,
        (seq_t)5808ULL,
        (seq_t)192752ULL,
        (seq_t)6443888ULL,
        (seq_t)215773456ULL,
        (seq_t)7226963248ULL,
        (seq_t)242067684912ULL,
        (seq_t)8108163611488ULL,
        (seq_t)271587161033712ULL
    };

    /* for n <= 9 just return the constant value */
    if (n <= 9) {
        return init[n-1];
    }

    /* initialise cache to hold at least indices 1..9 */
    if (len < 9) {
        size_t need = 10;                  /* indices 0..9 (0 unused) */
        if (need > cap) {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
            cap   = need;
        }
        for (size_t i = 1; i <= 9; ++i) {
            cache[i] = init[i-1];
        }
        len = 9;
    }

    /* enlarge cache array if required */
    if (n >= cap) {
        size_t newcap = n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute terms that are not yet stored */
    for (size_t i = len + 1; i <= n; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];
        seq_t v5 = cache[i-5];
        seq_t v6 = cache[i-6];
        seq_t v7 = cache[i-7];
        seq_t v8 = cache[i-8];
        seq_t v9 = cache[i-9];

        cache[i] = (seq_t)43 * v1
                 - (seq_t)319 * v2
                 - (seq_t)105 * v3
                 + (seq_t)4371 * v4
                 - (seq_t)4177 * v5
                 - (seq_t)9101 * v6
                 + (seq_t)12732 * v7
                 - (seq_t)5244 * v8
                 + (seq_t)704 * v9;
    }
    len = n;
    return cache[n];
}

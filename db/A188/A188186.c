/*
 * A188186  Number of strictly increasing arrangements of 8 numbers in
 *          -(n+6)..(n+6) with sum zero.
 * https://oeis.org/A188186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence * a(n) =
 *            3*a(n-1)-2*a(n-2)-3*a(n-4)+4*a(n-5)-3*a(n-8)+3*a(n-9) *
 *            -1*a(n-11)-1*a(n-12)+3*a(n-14)-3*a(n-15)+4*a(n-18) *
 *            -3*a(n-19)-2*a(n-21)+3*a(n-22)-1*a(n-23) * Initial values
 *            a(1)..a(24) are given below. The recurrence is valid * for all n
 *            ≥ 25
 * Range:     u8 none, u16 n <= 7, u32 n <= 55, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_seq[25] = {
    0,                      /* index 0 unused */
    289, 910, 2430, 5744,   /* a(1) .. a(4)   */
    12346, 24591, 46029, 81805,      /* a(5)..a(8)   */
    139143, 227930, 361384, 556834,  /* a(9)..a(12)  */
    836618, 1229093, 1769773, 2502617,/* a(13)..a(16) */
    3481445, 4771508, 6451232, 8614108, /* a(17)..a(20) */
    11370764, 14851235, 19207395,24615603 /* a(21)..a(24) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with first 24 terms on first call */
    if (!cache) {
        cache = (seq_t *)malloc((size_t)25 * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 1; i <= 24; ++i)
            cache[i] = init_seq[i];
        cache[0] = 0;
        cache_size = 25;
    }

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Expand cache to size n+1 */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute new terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 24) {          /* keep initial values if duplicated */
            cache[i] = init_seq[i];
            continue;
        }

        /* Compute using the empirical recurrence */
        seq_t pos = 0, neg = 0;

        /* Positive coefficient terms */
        pos += 3 * cache[i - 1];
        pos += 4 * cache[i - 5];
        pos += 3 * cache[i - 9];
        pos += 3 * cache[i - 14];
        pos += 4 * cache[i - 18];
        pos += 3 * cache[i - 22];

        /* Negative coefficient terms */
        neg += 2 * cache[i - 2];
        neg += 3 * cache[i - 4];
        neg += 3 * cache[i - 8];
        neg +=   cache[i - 11];
        neg +=   cache[i - 12];
        neg += 3 * cache[i - 15];
        neg += 3 * cache[i - 19];
        neg += 2 * cache[i - 21];
        neg +=   cache[i - 23];

        cache[i] = pos - neg;   /* pos ≥ neg by definition */
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}

/*
 * A211495  Number of (n+1) X (n+1) -4..4 symmetric matrices with every 2 X 2
 *          subblock having sum zero and three or four distinct values.
 * https://oeis.org/A211495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - a(n-2) - 75*a(n-3) + 78*a(n-4) + 268*a(n-5) -
 *            296*a(n-6) - 316*a(n-7) + 272*a(n-8) - 48*a(n-9) (empirical
 *            recurrence for n ≥ 10) Initial values (for 1 ≤ n ≤ 9): a(1)=28,
 *            a(2)=96, a(3)=336, a(4)=1208, a(5)=4360, a(6)=15792, a(7)=57128,
 *            a(8)=206424, a(9)=744656
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* sequence value type and index type are defined by the build */
static const seq_t init[9] = {
    (seq_t)28,  (seq_t)96,  (seq_t)336,  (seq_t)1208,  (seq_t)4360,
    (seq_t)15792, (seq_t)57128, (seq_t)206424, (seq_t)744656
};

/* cached computed values (1-indexed) */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_max = 0;   /* largest n for which cache[n] is valid */

/* enlarge cache to at least (n+1) elements, keeping previous data */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < n + 1) new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* main function requested by the build */
seq_t a(idx_t n)
{
    /* offset starts at 1 */
    if (n <= 0) return 0;                    /* undefined, return 0 */

    /* base cases from the initial table */
    if (n <= 9) return init[n - 1];

    /* initialise cache on first use */
    if (!cache) {
        ensure_capacity(9);
        for (idx_t i = 1; i <= 9; ++i) cache[i] = init[i - 1];
        cache_max = 9;
    }

    /* compute further values up to n */
    if (n > cache_max) {
        ensure_capacity(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t v = (seq_t)7 * cache[i - 1];
            v -= cache[i - 2];
            v -= (seq_t)75 * cache[i - 3];
            v += (seq_t)78 * cache[i - 4];
            v += (seq_t)268 * cache[i - 5];
            v -= (seq_t)296 * cache[i - 6];
            v -= (seq_t)316 * cache[i - 7];
            v += (seq_t)272 * cache[i - 8];
            v -= (seq_t)48 * cache[i - 9];
            cache[i] = v;
        }
        cache_max = n;
    }

    return cache[n];
}

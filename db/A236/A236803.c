/*
 * A236803  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) + 15*a(n-2) - 672*a(n-3) + 2434*a(n-4) +
 *            864*a(n-5) - 17988*a(n-6) + 24192*a(n-7) + 14472*a(n-8) -
 *            46656*a(n-9) + 23328*a(n-10) Initial values (a(1) … a(10)): 277,
 *            1435, 8825, 60187, 410305, 2926867, 20377961, 147734251,
 *            1039350577, 7613312515. The function memoises the values so...
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[10] = {
    (seq_t)277ULL,
    (seq_t)1435ULL,
    (seq_t)8825ULL,
    (seq_t)60187ULL,
    (seq_t)410305ULL,
    (seq_t)2926867ULL,
    (seq_t)20377961ULL,
    (seq_t)147734251ULL,
    (seq_t)1039350577ULL,
    (seq_t)7613312515ULL
};

static seq_t *cache = NULL;          /* cache[0] is unused, indices 1..n contain a(n) */
static idx_t  cache_last  = 0;        /* highest index that has been computed */
static size_t cache_cap  = 0;        /* capacity of cache array (number of usable slots) */

static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    /* grow at least 64 slots beyond requested n for amortised guarantees */
    size_t new_cap = (size_t)n + 64;
    seq_t *new_ptr = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
    if (cache_last == 0) {
        /* first allocation: initialise 1..10 */
        cache[0] = 0;
        for (idx_t i = 1; i <= 10 && i <= new_cap; ++i)
            cache[i] = init_vals[i - 1];
        cache_last = 10;
    }
}

extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= 10) return init_vals[n - 1];

    ensure_capacity(n);

    if (n <= cache_last) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term =
            ((seq_t)12   * cache[i - 1]) +
            ((seq_t)15   * cache[i - 2]) +
            ((seq_t)2434 * cache[i - 4]) +
            ((seq_t)864  * cache[i - 5]) +
            ((seq_t)24192 * cache[i - 7]) +
            ((seq_t)14472 * cache[i - 8]) +
            ((seq_t)23328 * cache[i -10]) -
            (((seq_t)672  * cache[i - 3]) +
             ((seq_t)17988 * cache[i - 6]) +
             ((seq_t)46656 * cache[i - 9]));
        cache[i] = term;
    }
    cache_last = n;
    return cache[n];
}

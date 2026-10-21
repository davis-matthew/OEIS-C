/*
 * A235898  Number of (n+1)X(4+1) 0..2 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every
 *          2X2 subblock.
 * https://oeis.org/A235898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235898:
 * a(n) = 33*a(n-1) -312*a(n-2) -360*a(n-3) +16976*a(n-4)
 *        -22128*a(n-5) -360576*a(n-6) +253440*a(n-7)
 *        +3520512*a(n-8) +3870720*a(n-9)
 * Initial values:
 * a(1)=1797, a(2)=17001, a(3)=148945, a(4)=1384257,
 * a(5)=12931393, a(6)=124394337, a(7)=1221438401,
 * a(8)=12293017761, a(9)=126561018497
 */

#include <stdlib.h>
#include <stdint.h>

/* Initial 9 terms of the sequence */
static const seq_t init_vals[9] = {
    (seq_t)1797ULL,
    (seq_t)17001ULL,
    (seq_t)148945ULL,
    (seq_t)1384257ULL,
    (seq_t)12931393ULL,
    (seq_t)124394337ULL,
    (seq_t)1221438401ULL,
    (seq_t)12293017761ULL,
    (seq_t)126561018497ULL
};

/* Coefficients of the 9‑term recurrence (signed 64‑bit integers) */
static const int64_t coeffs[9] = {
    33LL,
    -312LL,
    -360LL,
    16976LL,
    -22128LL,
    -360576LL,
    253440LL,
    3520512LL,
    3870720LL
};

/* Dynamic cache for memoization */
static seq_t *cache = NULL;      /* cache[0] holds a(1) */
static size_t cache_size = 0;    /* number of terms stored so far */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Ensure that the cache can store at least `needed` elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();  /* memory allocation failed */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        abort();  /* the sequence is defined only for n >= 1 */

    /* Initialize cache on first call */
    if (cache == NULL) {
        ensure_cache_size(9);
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_size = 9;
    }

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Compute missing terms up to n */
    ensure_cache_size((size_t)n);
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 0; k < 9; ++k) {
            seq_t term = cache[i - 1 - k];
            int64_t c = coeffs[k];
            if (c > 0)
                pos_sum += term * (seq_t)c;
            else
                neg_sum += term * (seq_t)(-c);
        }
        /* Result is guaranteed to be non‑negative */
        cache[i] = pos_sum - neg_sum;
    }
    cache_size = (size_t)n;
    return cache[n - 1];
}

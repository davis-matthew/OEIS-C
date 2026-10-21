/*
 * A229822  Even bisection gives sequence a itself, n->a(2*(7*n+k)-1) gives
 *          k-th differences of a for k=1..7 with a(n)=n for n<2.
 * https://oeis.org/A229822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   not stated
 * Range:     i8 n <= 68, i16 n <= 1902, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1;
 * a(2*n)              = a(n);
 * a(2*(7*n+k)-1) = Σ_{j=0..k} (-1)^j * C(k,j) * a(n+k-j)   for k=1..7
 * (indices n,k≥0)
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* table of binomial coefficients C(k,j) for 0 ≤ k ≤ 7 */
static const unsigned char C[8][8] = {
    {1},
    {1,1},
    {1,2,1},
    {1,3,3,1},
    {1,4,6,4,1},
    {1,5,10,10,5,1},
    {1,6,15,20,15,6,1},
    {1,7,21,35,35,21,7,1}
};

/* static cache for memoised values */
static seq_t *cache = NULL;      /* array of computed a(n) */
static size_t cache_cap = 0;     /* allocated size of cache */
static idx_t computed_max = 0;   /* highest n that has been computed */

/* enlarge cache to hold indices up to at least n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_cap > n) return;
    size_t old_cap = cache_cap;
    size_t new_cap = n + 1;
    if (new_cap < 2 * old_cap) new_cap = 2 * old_cap;
    size_t old_bytes = old_cap * sizeof(seq_t);
    size_t new_bytes = new_cap * sizeof(seq_t);
    seq_t *newp;
    if (!cache) {
        newp = (seq_t *)malloc(new_bytes);
    } else {
        newp = (seq_t *)realloc(cache, new_bytes);
    }
    if (!newp) {
        /* allocation failed – terminate program (allowed in tests) */
        abort();
    }
    cache = newp;
    cache_cap = new_cap;
    if (old_cap < new_cap) {
        /* zero-initialise new area – not needed but safe */
        memset((void *)((char *)cache + old_cap * sizeof(seq_t)), 0,
               (new_cap - old_cap) * sizeof(seq_t));
    }
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        computed_max = 1;
    }

    if (n > computed_max) {
        ensure_capacity(n);
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i < 2) {               /* values for 0 and 1 already set */
                cache[i] = (seq_t)i;
                continue;
            }
            if ((i & 1) == 0) {        /* even index */
                cache[i] = cache[i >> 1];
            } else {                   /* odd index */
                /* write i as: i = 2*(7*n + k) - 1   with 1 ≤ k ≤ 7   */
                idx_t t = (i + 1) >> 1;   /* t = 7*n + k   */
                idx_t m = (t - 1) / 7;    /* n = ((t-1)/7) */
                idx_t k = t - 7 * m;      /* 1 ≤ k ≤ 7     */
                seq_t sum = 0;
                for (idx_t j = 0; j <= k; ++j) {
                    idx_t idx   = m + k - j;           /* location of a(*) in sum */
                    seq_t coeff = (j & 1) ? (seq_t)-1 : (seq_t)1;
                    seq_t binom = (seq_t)C[(int)k][(int)j];
                    seq_t val   = cache[idx];
                    sum += coeff * binom * val;
                }
                cache[i] = sum;
            }
        }
        computed_max = n;
    }
    return cache[n];
}

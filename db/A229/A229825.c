/*
 * A229825  Even bisection gives sequence a itself, n->a(2*(10*n+k)-1) gives
 *          k-th differences of a for k=1..10 with a(n)=n for n<2.
 * https://oeis.org/A229825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   recursive definition a(0)=0, a(1)=1 a(2*n)=a(n)
 *            a(2*(10*n+k)-1)=∑_{j=0..k} (-1)^j * C(k,j) * a(n + k - j) for
 *            k=1..10
 * Range:     i8 n <= 18, i16 n <= 278, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* binomial coefficients up to 10 */
static const int binom[11][11] = {
   {1,0,0,0,0,0,0,0,0,0,0},
   {1,1,0,0,0,0,0,0,0,0,0},
   {1,2,1,0,0,0,0,0,0,0,0},
   {1,3,3,1,0,0,0,0,0,0,0},
   {1,4,6,4,1,0,0,0,0,0,0},
   {1,5,10,10,5,1,0,0,0,0,0},
   {1,6,15,20,15,6,1,0,0,0,0},
   {1,7,21,35,35,21,7,1,0,0,0},
   {1,8,28,56,70,56,28,8,1,0,0},
   {1,9,36,84,126,126,84,36,9,1,0},
   {1,10,45,120,210,252,210,120,45,10,1}
};

/* cache for already computed terms */
static seq_t *cache      = NULL; /* sequence values */
static size_t cache_cap  = 0;    /* allocated size */
static idx_t  cached_up_to = 0;  /* largest index computed so far (inclusive) */

/* Ensure the cache can hold at least indices up to n. */
static void grow_cache(idx_t n)
{
    if ((idx_t)cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap*2 : 16;
    while ((idx_t)new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Grow cache if needed */
    grow_cache(n);

    /* Initialise a(0) once */
    cache[0] = 0;

    /* Compute any missing values up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;
        } else if ((i & 1) == 0) {            /* even i: a(2*n)=a(n) */
            cache[i] = cache[i/2];
        } else {                              /* odd i > 1 */
            idx_t t   = (i + 1) >> 1;          /* t = (i+1)/2 */
            int   k   = t % 10;
            if (k == 0) k = 10;               /* k in 1..10 */
            idx_t n0  = (t - k) / 10;         /* base n for the recurrence */

            seq_t sum = 0;
            for (int j = 0; j <= k; ++j) {
                idx_t idx = n0 + k - j;       /* index of a() in the sum */
                seq_t val = cache[idx];
                seq_t mul = (seq_t)binom[k][j] * val;
                if (j & 1) sum -= mul;        /* (-1)^j factor */
                else       sum += mul;
            }
            cache[i] = sum;
        }
    }

    cached_up_to = (cached_up_to < n) ? n : cached_up_to;
    return cache[n];
}

/*
 * A188101  Number of (n+3) X 8 binary arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A188101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 61, u64 n <= 151, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Empirical recurrence: 
   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) + a(n-8) - a(n-9)
   for n > 15.
   Initial terms a(1) … a(15) are hard‑coded. */

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static size_t cache_cap = 0;         /* allocated number of elements */
static idx_t cache_used = 0;         /* how many terms are already stored */

static const seq_t init[15] = {
    (seq_t)8274,  (seq_t)1482,  (seq_t)3013,  (seq_t)5037,  (seq_t)7661,
    (seq_t)8509,  (seq_t)8873,  (seq_t)9265,  (seq_t)9825,  (seq_t)10805,
    (seq_t)13085, (seq_t)16429, (seq_t)22789, (seq_t)30417, (seq_t)41390
};

/* Compute terms up to (but not including) n, assuming n>=1. */
static void ensure_upto(idx_t n)
{
    if (n <= cache_used)
        return;                     /* already known */

    /* grow storage if needed */
    if (cache_cap < (size_t)n) {
        size_t newcap = cache_cap ? cache_cap : 16;
        while (newcap < (size_t)n)
            newcap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache     = p;
        cache_cap = newcap;
    }

    /* compute missing terms */
    for (idx_t i = cache_used; i < n; ++i) {
        if (i < 15) {                /* initial terms are known */
            cache[i] = init[i];
        } else {                     /* recurrence for i >= 15 (i = n-1) */
            /* 2*a(i) - a(i-1) + a(i-3) - 2*a(i-4) + a(i-5) + a(i-7) - a(i-8) */
            seq_t sum1 = cache[i - 1] + cache[i - 1];   /* 2*a(i)   */
            sum1 += cache[i - 4];                       /* +a(i-3)  */
            sum1 += cache[i - 6];                       /* +a(i-5)  */
            sum1 += cache[i - 8];                       /* +a(i-7)  */

            seq_t sum2 = cache[i - 2];                  /* a(i-1)   */
            seq_t tmp   = cache[i - 5] + cache[i - 5];  /* 2*a(i-4) */
            sum2 += tmp;
            sum2 += cache[i - 9];                       /* +a(i-8)  */

            cache[i] = sum1 - sum2;                     /* final value */
        }
    }
    cache_used = n;
}

/* Public export required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n >= 1 */
        return 0;
    ensure_upto(n);
    return cache[n - 1]; /* array is 0‑based, n is 1‑based */
}

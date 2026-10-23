/*
 * A177183  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=-1 and l=-1.
 * https://oeis.org/A177183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by the recurrence formula
   a(0) = 1, a(1) = 8
   a(n+1) = Σ_{p=0}^{n} (a(p)*a(n-p) + k) + l
   for k = -1, l = -1, i.e.
   a(m) = Σ_{p=0}^{m-1} a(p)*a(m-1-p) - (m+1)   for m ≥ 2

   For n ≥ 6 the following linear recurrence holds:
   (n+1)a(n)            - (7*n-2)a(n-1)   - (13*n-35)a(n-2)
   +   (83*n-262)a(n-3) + (-100*n+404)a(n-4) + 36*(n-5)a(n-5) = 0
   so that
   a(n) = [(7*n-2)a(n-1) + (13*n-35)a(n-2) - (83*n-262)a(n-3)
           + (100*n-404)a(n-4) - 36*(n-5)a(n-5)] / (n+1)
   This formula is used for n ≥ 6.  For n < 6 the direct convolution
   formula above is used. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Allocate or enlarge the cache to at least `cap` elements. */
static void ensure_cache_cap(size_t cap)
{
    if (cache_cap >= cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : cap;
    while (new_cap < cap)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise cache with the first two terms of the sequence. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache = (seq_t *)calloc(4, sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache_cap = 4;
        cache_len = 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }
}

/* Compute a(n) and store it in the cache. */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    init_cache();

    /* enlarge cache if necessary */
    size_t needed = (size_t)n + 1;
    ensure_cache_cap(needed);

    /* compute missing values up to n */
    for (size_t m = cache_len; m <= (size_t)n; ++m) {
        if (m == 0 || m == 1) {
            /* already known */
            continue;
        } else if (m < 6) {
            /* direct convolution for the first few terms */
            seq_t sum = 0;
            for (size_t p = 0; p < m; ++p) {
                sum += cache[p] * cache[m - 1 - p];
            }
            sum -= (seq_t)(m + 1);   /* subtract (m+1) = (n+2) for a(m) */
            cache[m] = sum;
        } else {
            /* linear recurrence for n ≥ 6 */
            idx_t mm = (idx_t)m;

            seq_t t1 = cache[m - 1] * (seq_t)(7   * mm - 2);
            seq_t t2 = cache[m - 2] * (seq_t)(13  * mm - 35);
            seq_t t3 = cache[m - 3] * (seq_t)(83  * mm - 262);
            seq_t t4 = cache[m - 4] * (seq_t)(100 * mm - 404);
            seq_t t5 = cache[m - 5] * (seq_t)(36  * (mm - 5));

            seq_t sum = t1 + t2 + t4;   /* start with the positive terms */
            sum -= t3;                  /* subtract the negative term */
            sum -= t5;                  /* subtract the last positive term */

            cache[m] = sum / (seq_t)(mm + 1);
        }
    }
    cache_len = needed;
    return cache[(size_t)n];
}

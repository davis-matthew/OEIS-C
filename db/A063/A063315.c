/*
 * A063315  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          42 ).
 * https://oeis.org/A063315
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) - a(n-3) +
 *            2*a(n-4) - a(n-5) + a(n-7) - a(n-8) + b(n) where b(n) =
 *            coefficient of x^n in N(x) = -x^2*(x^9 -15*x^7 -9*x^6 -11*x^5
 *            -25*x^4 -9*x^3 -11*x^2 -14*x +1) i.e. b(0)=b(1)=0, b(2)=-1,
 *            b(3)=14, b(4)=11, b(5)=9, b(6)=25, b(7)=11, b(8)=9, b(9)=...
 * Range:     i8 n <= 13, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=0 are implied by the recurrence.  */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* array of computed terms a(k) */
static size_t cache_len = 0;  /* largest index computed so far + 1 */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure the cache is large enough to hold indices up to 'limit' (inclusive). */
static void ensure_cache(size_t limit)
{
    if (cache_cap > limit)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= limit)
        new_cap <<= 1;
    void *ptr = realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) abort();
    cache = (seq_t*)ptr;
    cache_cap = new_cap;
    /* no need to initialize new entries; they will be written sequentially */
}

/* Ncoeff[n] for 0 <= n <= 11, otherwise 0 */
static const seq_t Ncoeff[12] = {
    0,   /* n = 0 */
    0,   /* n = 1 */
    -1,  /* n = 2 */
    14,  /* n = 3 */
    11,  /* n = 4 */
    9,   /* n = 5 */
    25,  /* n = 6 */
    11,  /* n = 7 */
    9,   /* n = 8 */
    15,  /* n = 9 */
    0,   /* n = 10 */
   -1    /* n = 11 */
};

seq_t a(idx_t n)
{
    if (n < 0) return 0;   /* undefined for negative indices, but safe */
    size_t idx = (size_t)n;
    if (idx >= cache_len)
    {
        ensure_cache(idx);
        /* compute terms up to idx sequentially */
        for (size_t k = cache_len; k <= idx; ++k)
        {
            seq_t val = 0;
            if (k >= 1) val += cache[k-1];
            if (k >= 3) val -= cache[k-3];
            if (k >= 4) val += (seq_t)(2) * cache[k-4];
            if (k >= 5) val -= cache[k-5];
            if (k >= 7) val += cache[k-7];
            if (k >= 8) val -= cache[k-8];
            if (k <= 11) val += Ncoeff[k];
            cache[k] = val;
        }
        cache_len = idx + 1;
    }
    return cache[idx];
}

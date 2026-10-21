/*
 * A052632  E.g.f. 1/(1-x-x^5).
 * https://oeis.org/A052632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052632
 *  E.g.f.: 1/(1 - x - x^5)
 *
 *  Let b(n) be the coefficients of the EGF so that
 *    sum_{n>=0} b(n) x^n  = 1/(1 - x - x^5)
 *  Then b(0) = 1 and for n≥1   b(n) = b(n-1) + b(n-5)
 *  (with b(k)=0 for k<0).
 *  The desired values are a(n) = n! * b(n).
 *
 *  From the relation b(n)=b(n-1)+b(n-5) we obtain the integer
 *  recurrence for a(n) (valid for n≥5)
 *
 *      a(n) = n * a(n-1) + n*(n-1)*(n-2)*(n-3)*(n-4) * a(n-5)
 *
 *  The implementation uses memoisation (static cache) so that
 *  each term is computed only once, independent of call order.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] = a(i) */
static idx_t cache_len = 0;   /* number of allocated slots */

/* initialise first five terms */
static void init_cache(void)
{
    if (cache_len > 0) return;  /* already initialised */

    /* allocate space for at least 5 elements */
    cache_len = 5;
    cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
    if (!cache) return;  /* out of memory – caller will see zero */

    cache[0] = (seq_t)1;   /* a(0) */
    cache[1] = (seq_t)1;   /* a(1) */
    cache[2] = (seq_t)2;   /* a(2) */
    cache[3] = (seq_t)6;   /* a(3) */
    cache[4] = (seq_t)24;  /* a(4) */
}

/* extend cache to at least (n+1) elements */
static void extend_cache(idx_t n)
{
    if (n + 1 <= cache_len) return;

    idx_t new_len = cache_len > 0 ? cache_len : 5;
    while (new_len <= n) new_len <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return;   /* out of memory – caller will see zero */

    /* clear newly allocated part */
    for (idx_t i = cache_len; i < new_len; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_len = new_len;
}

static void compute_terms(idx_t upto)
{
    /* assume cache[0..cached_up_to-1] already filled */
    static idx_t cached_up_to = 0;

    if (cached_up_to > upto) return;

    for (idx_t i = cached_up_to; i <= upto; ++i)
    {
        if (i < 5) continue;    /* already initialised */

        /* compute product n*(n-1)*...*(n-4) */
        seq_t prod = (seq_t)i;
        prod *= (seq_t)(i-1);
        prod *= (seq_t)(i-2);
        prod *= (seq_t)(i-3);
        prod *= (seq_t)(i-4);

        seq_t term1 = (seq_t)i * cache[i-1];
        seq_t term2 = prod * cache[i-5];
        cache[i] = term1 + term2;
    }
    cached_up_to = upto + 1;
}

seq_t a(idx_t n)
{
    init_cache();
    extend_cache(n);
    compute_terms(n);
    return cache[n];
}

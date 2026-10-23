/*
 * A170577  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) =
 *            38*(a(n-1)+a(n-2)+…+a(n-46)) – 741*a(n-47) + b(n) * where
 *            b(0)=1, b(1)=…=b(46)=2, b(47)=1 and b(n)=0 for n>47. * The
 *            recurrence comes from the rational generating function * (t^47 +
 *            2t^46 + … + 2t + 1)/(741t^47 – 38t^46 – … – 38t + 1). * An
 *            implementa...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values */
static idx_t  cache_size = 0;      /* number of entries in cache */

/* Coefficients of the numerator (b(n)) up to degree 47 */
static seq_t *b_coeff = NULL;      /* size 48 (indices 0..47) */

/* initialise coefficient array */
static void init_coeffs(void)
{
    if (b_coeff != NULL)
        return;
    b_coeff = (seq_t *)malloc(48 * sizeof(seq_t));
    if (!b_coeff)
        abort(); /* out of memory */

    b_coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= 46; ++i)
        b_coeff[i] = (seq_t)2;
    b_coeff[47] = (seq_t)1;
}

/* extend the cache array to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!p)
        abort(); /* out of memory */
    cache = p;
    cache_size = new_size;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    init_coeffs();

    ensure_cache(n);

    /* compute missing values in the cache (if any) */
    for (idx_t i = 0; i < n; ++i) {
        /* value already computed */
    }

    /* start computing from the first missing index */
    for (idx_t i = 0; i < n; ++i) { /* nothing here; values are already set */
    }
    for (idx_t i = 0; i < n; ++i) {
        /* do nothing; keep for clarity */
    }

    /* compute up to n */
    for (idx_t i = 0; i <= n; ++i) {
        /* skip already computed entries */
        if (i < cache_size && i < (idx_t)cache_size)
            continue;
    }

    /* The above loops are unnecessary; we compute from the smallest
       missing index up to n.  Since we start with an empty cache,
       we simply compute from 0 to n.  */
    /* For safety, recompute the whole cache from scratch for the first call.   */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* contributions from a[i-1]..a[i-46] with factor 38 */
        for (idx_t j = 1; j <= 46 && i >= j; ++j)
            sum += (seq_t)38 * cache[i - j];

        /* contribution from a[i-47] with factor -741 (subtracted) */
        if (i >= 47)
            sum -= (seq_t)741 * cache[i - 47];

        /* add b(i) */
        seq_t b = (i <= 47) ? b_coeff[i] : (seq_t)0;
        cache[i] = sum + b;
    }

    return cache[n];
}

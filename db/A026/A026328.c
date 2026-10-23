/*
 * A026328  a(n) = number of (s(0), s(1), ..., s(n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| <= 1 for i = 1,2,...,n,
 *          s(0) = 2, s(n) = 5. Also a(n) = T(n,n-3), where T is the array in
 *          A026323.
 * https://oeis.org/A026328
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the (conjectured) recurrence for n ≥ 7 * a(n) =
 *            ((6n²+21n−128)·a(n−1) + 12(n−1)(n−2)·a(n−4) * −
 *            (9n²+3n−152)·a(n−2) − 4(n+4)(n−1)·a(n−3)) * / ((n+9)(n−3)) *
 *            Initial values: a(3)=1, a(4)=4, a(5)=15, a(6)=50
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache      = NULL;           // cached values a(0)…a(n)
static idx_t  cache_size = 0;              // size of allocated cache
static idx_t  max_comp   = 0;              // largest n for which a(n) is computed

/* Ensure that cache is valid up to index n (n >= 3) */
static void ensure_cache(idx_t n)
{
    if (n < 3)
        return;                          /* values below offset are irrelevant */

    /* Allocate cache on first call */
    if (!cache) {
        size_t need = (size_t)n + 1;      /* indices 0..n inclusive */
        if (need < 7)
            need = 7;                    /* we need up to a(6) for the recursion */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache)
            return;                      /* allocation failed – caller will use 0 */
        memset(cache, 0, need * sizeof(seq_t));
        /* initialise base values (a(3)..a(6)) */
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)15;
        cache[6] = (seq_t)50;
        cache_size = need;
        max_comp   = 6;
        return;
    }

    /* Grow cache if needed */
    if ((size_t)cache_size <= (size_t)n) {
        size_t newsize = cache_size ? cache_size : 8;
        while (newsize <= (size_t)n)
            newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp)
            return;                      /* allocation failed – caller will use 0 */
        /* Zero the newly allocated part */
        memset(tmp + cache_size, 0, (newsize - cache_size) * sizeof(seq_t));
        cache     = tmp;
        cache_size = newsize;
    }

    /* Compute the missing values using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* values up to a(6) are already set */
        if (i <= 6)
            continue;

        /* fetch previously computed terms */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        /* coefficients (all fit in 64 bits) */
        seq_t coeff1 = (seq_t)(6 * i * i + 21 * i - 128);           /* positive */
        seq_t coeff4 = (seq_t)(12 * (i - 1) * (i - 2));            /* positive */
        seq_t coeff2 = (seq_t)(9 * i * i + 3 * i - 152);           /* used as |negative| */
        seq_t coeff3 = (seq_t)(4 * (i + 4) * (i - 1));              /* used as |negative| */

        /* build positive and negative parts of the numerator */
        seq_t pos = coeff1 * a1 + coeff4 * a4;
        seq_t neg = coeff2 * a2 + coeff3 * a3;

        /* numerator = pos - neg (should be non‑negative) */
        seq_t numer = pos - neg;

        /* denominator */
        seq_t denom = (seq_t)((i + 9) * (i - 3));

        /* store result */
        cache[i] = numer / denom;
    }

    if (n > max_comp)
        max_comp = n;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;                   /* values below offset are irrelevant */
    ensure_cache(n);
    return cache[n];
}

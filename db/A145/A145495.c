/*
 * A145495  Coefficients of certain power series associated with Atkin
 *          polynomials (see Kaneko-Zagier reference for precise definition).
 * https://oeis.org/A145495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A145495 – coefficients of Atkin polynomials
 *
 *  Formula (recurrence):
 *      a(0) = 1
 *      a(1) = 84
 *      a(n) = 12 · (6n-6+(-1)^(n-1)) · (6n+(-1)^(n-1)) · a(n-1) / ((n-1)·n)
 *           ,  n > 1
 *
 *  The function uses a memoised recurrence.  It reduces intermediate
 *  products by GCDs before the final multiplication to keep the
 *  numbers as small as possible.  All arithmetic is done with the
 *  arbitrary‑precision `seq_t` type and the helper functions
 *  `seq_gcd` and `seq_mul` from the utils directory.
 */

#include <stdlib.h>
#include "../utils/seq_gcd.h"   /* seq_gcd */
#include "../utils/seq_mul.h"   /* seq_mul */

/* cache for previously computed terms */
static seq_t *cache = NULL;      /* allocated array of seq_t */
static idx_t  cache_len = 0;     /* current number of stored terms */
static size_t cache_cap = 0;     /* capacity of the allocated array */

static void grow_cache(idx_t n)
{
    /* ensure cache holds at least n+1 elements */
    idx_t needed = n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)needed)
            new_cap = (size_t)needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();               /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)84;

    /* initialise cache with the first two terms */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)84;
        cache_len = 2;
    }

    /* grow cache if necessary */
    grow_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];          /* a(i-1) */
        /* denominator (i-1)*i */
        seq_t den = (seq_t)(i - 1) * (seq_t)i;

        /* sign = (-1)^(i-1) */
        int sign = ((i - 1) & 1) ? -1 : 1;

        /* numerator factors */
        seq_t f1 = (seq_t)12;
        seq_t f2 = (seq_t)(6 * i - 6 + sign);
        seq_t f3 = (seq_t)(6 * i + sign);

        /* cancel common factors between prev and the denominator */
        seq_t g = seq_gcd(prev, den);
        if (g != (seq_t)1) {
            prev /= g;
            den /= g;
        }

        /* process factor f1 */
        g = seq_gcd(f1, den);
        if (g != (seq_t)1) {
            f1 /= g;
            den /= g;
        }
        prev = seq_mul(prev, f1);           /* multiply safely */
        g = seq_gcd(prev, den);
        if (g != (seq_t)1) {
            prev /= g;
            den /= g;
        }

        /* process factor f2 */
        g = seq_gcd(f2, den);
        if (g != (seq_t)1) {
            f2 /= g;
            den /= g;
        }
        prev = seq_mul(prev, f2);
        g = seq_gcd(prev, den);
        if (g != (seq_t)1) {
            prev /= g;
            den /= g;
        }

        /* process factor f3 */
        g = seq_gcd(f3, den);
        if (g != (seq_t)1) {
            f3 /= g;
            den /= g;
        }
        prev = seq_mul(prev, f3);
        g = seq_gcd(prev, den);
        if (g != (seq_t)1) {
            prev /= g;
            den /= g;
        }

        /* den should now be 1; store the computed term */
        cache[i] = prev;
    }

    cache_len = n + 1;   /* all terms up to n are now stored */
    return cache[n];
}

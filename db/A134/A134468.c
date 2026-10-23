/*
 * A134468  Numbers n such that 2^n and 3^n have the same leading digit.
 * https://oeis.org/A134468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  2^n and 3^n have the same leading digit.  The sequence lists
    the exponents n (starting with a(1)=0) for which the first
    decimal digit of pow(2,n) and pow(3,n) coincide.  */

/*  The implementation iteratively searches for such exponents,
    caching the results so that each element is generated only once.
    Leading digits are obtained from the fractional part of
    n·log10(2) and n·log10(3).  long double precision guarantees
    correctness for all n up to at least 10 000.  */

#include <stdlib.h>
#include <math.h>

static const long double LOG10_2 = log10l((long double)2);
static const long double LOG10_3 = log10l((long double)3);

/* Return the leading decimal digit of  b^e  (b=2 or 3) */
static inline int leading_digit(idx_t e, long double log10_base)
{
    long double frac = (long double)e * log10_base;
    frac -= floorl(frac);               /* keep fractional part */
    long double val  = powl(10.0L, frac) + 1e-15L; /* guard against 10.0 */
    return (int)(val);                  /* truncation gives 1..9 */
}

/*  Cache of exponents for which 2^n and 3^n share a leading digit. */
static seq_t *cache       = NULL;      /* array of matches          */
static size_t cache_len   = 0;         /* number of stored matches  */
static size_t cache_cap   = 0;         /* allocated array capacity  */
static idx_t  last_checked = 0;        /* exponent most recently tested */

/*  Ensure enough room for new cache entries.  */
static void ensure_capacity(void)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
    }
}

seq_t a(idx_t n)
{
    /*  n is 1‑based; a(1)=0 is always the first match.  */
    if (n == 0) return 0;   /* the sequence is defined for n>=1, but guard   */

    /*  Generate matches until the required index is available.  */
    while (cache_len < n) {
        /*  First element: exponent 0 satisfies the condition.  */
        if (cache_len == 0) {
            ensure_capacity();
            cache[0] = (seq_t)0;
            cache_len = 1;
            last_checked = 0;
            continue;
        }

        /*  Test the next exponent.  */
        idx_t e = last_checked + 1;
        int d2 = leading_digit(e, LOG10_2);
        int d3 = leading_digit(e, LOG10_3);

        if (d2 == d3) {
            ensure_capacity();
            cache[cache_len++] = (seq_t)e;
        }
        last_checked = e;   /* remember what we have examined   */
    }

    return cache[n - 1];
}

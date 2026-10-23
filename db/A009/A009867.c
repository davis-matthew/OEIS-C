/*
 * A009867  Coordination sequence T2 for Zeolite Code AHT.
 * https://oeis.org/A009867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 172, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009867 – coordination sequence T2 for Zeolite Code AHT.
   Generating function:
       (1 + 2x + 4x² + 4x³ + 10x⁴ + 11x⁵ + 14x⁶ + 13x⁷ + 20x⁸ + 13x⁹
        + 14x¹⁰ + 12x¹¹ + 14x¹² + 4x¹³ + 2x¹⁴ + x¹⁵ + 3x¹⁶ - x¹⁷ - 2x¹⁸ + x²¹)
    -------------- 
    ((1 - x)³ (1 + x) (1 + x²)² (1 + x⁴)²)
   Hence
       (1 - 2x + 2x² - 2x³ + 2x⁴ - 2x⁵ + 2x⁶ - 2x⁷ + 0x⁸ + 2x⁹ - 2x¹⁰ + 2x¹¹
        - 2x¹² + 2x¹³ - 2x¹⁴ + 2x¹⁵ - x¹⁶) · A(x)
   = 1 + 2x + 4x² + 4x³ + 10x⁴ + 11x⁵ + 14x⁶ + 13x⁷ + 20x⁸ + 13x⁹
     + 14x¹⁰ + 12x¹¹ + 14x¹² + 4x¹³ + 2x¹⁴ + x¹⁵ + 3x¹⁶ - x¹⁷ - 2x¹⁸ + x²¹
   Therefore for all n ≥ 0:
       a(n) = Σ_{k=1}^{16} c[k] · a(n-k) + p[n]
   where
       c[1..16] = { 2, -2,  2, -2,  2, -2,  2, 0, -2,  2, -2,  2, -2,  2, -2,  1 },
   and p[n] is the coefficient of xⁿ in the numerator (p[n] = 0 for n > 21). */

/* Dependencies:  seq_t and idx_t are defined by the build system.  */
#include <stdlib.h>
#include <stdint.h>

/* recurrence coefficients c[k] = -coeff of x^k in the denominator polynomial */
static const int c[17] = {0,  2, -2,  2, -2,  2, -2,  2,  0,
                          -2,  2, -2,  2, -2,  2, -2,  1};

/* coefficients of the numerator polynomial p[n], 0 for n > 21 */
static const int p[22] = {1, 2, 4, 4, 10, 11, 14, 13, 20, 13,
                          14, 12, 14, 4, 2, 1, 3, -1, -2, 0, 0, 1};

/* static cache of computed terms */
static seq_t *cache = NULL;     /* pointer to allocated array */
static idx_t cache_size = 0;    /* number of allocated slots */
static idx_t last_computed = (idx_t)(-1);   /* largest index that has a value */

/* allocate (or reallocate) cache up to at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_size) return;   /* already large enough */

    idx_t new_size = n + 1;
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_size];          /* C++: constructor called */
#else
    new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
    if (!new_cache) abort();                  /* memory allocation failed */

    /* copy existing values */
    if (cache) {
        for (idx_t i = 0; i <= last_computed; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0 && last_computed == (idx_t)(-1)) { /* special case when no term computed yet */
        ensure_cache(0);
    }
    ensure_cache(n);

    /* compute terms from (last_computed+1) up to n */
    idx_t start = (last_computed == (idx_t)(-1)) ? 0 : last_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = 0;
        /* linear recurrence part */
        for (idx_t k = 1; k <= 16 && i >= k; ++k) {
            seq_t t = cache[i - k];
            int ck = c[k];
            switch (ck) {
            case  2: sum += t + t;   break;
            case -2: sum -= t + t;   break;
            case  1: sum += t;       break;
            case -1: sum -= t;       break;
            case  0: /* nothing */   break;
            }
        }
        /* add numerator contribution if inside its range */
        if (i < 22) {
            int pn = p[i];
            if (pn >= 0)
                sum += (seq_t)pn;
            else
                sum -= (seq_t)(-pn);
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}

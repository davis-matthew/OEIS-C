/*
 * A143471  Expansion of 1/(x^10*p(x + 1/x)), where p(x) = 1 - x^3 - x^5 - x^7
 *          + x^10 is a Salem polynomial.
 * https://oeis.org/A143471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 17, i32 n <= 40, i64 n <= 86, i128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -10*a(n-2)+a(n-3)-45*a(n-4)+8*a(n-5)-120*a(n-6)+27*a(n-7)
   -210*a(n-8)+48*a(n-9)-253*a(n-10)+48*a(n-11)-210*a(n-12)+27*a(n-13)
   -120*a(n-14)+8*a(n-15)-45*a(n-16)+a(n-17)-10*a(n-18)-a(n-20)
   initial values: a(0)=1, a(1)=0, a(2)=-10, a(3)=1, a(4)=55, a(5)=-12,
   a(6)=-219, a(7)=77, a(8)=701, a(9)=-351, a(10)=-1900, a(11)=1277,
   a(12)=4494, a(13)=-3966, a(14)=-9485, a(15)=11058, a(16)=18342,
   a(17)=-29012, a(18)=-34057, a(19)=75053. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Base initial values a(0)…a(19). */
static const seq_t base[20] = {
    1,   0,   -10,   1,   55,   -12,  -219,  77,  701, -351,
   -1900, 1277, 4494, -3966, -9485, 11058, 18342, -29012, -34057, 75053
};

/* Coefficients for the recurrence a(n) = Σ_{i=2}^{20} coeff[i]·a(n−i). */
static const int coeff[21] = {
    0,    /* a(n)      */
    0,    /* a(n-1)    */
   -10,   /* a(n-2)    */
    1,    /* a(n-3)    */
   -45,   /* a(n-4)    */
    8,    /* a(n-5)    */
  -120,   /* a(n-6)    */
    27,   /* a(n-7)    */
  -210,   /* a(n-8)    */
    48,   /* a(n-9)    */
 -253,   /* a(n-10)   */
    48,   /* a(n-11)   */
  -210,   /* a(n-12)   */
    27,   /* a(n-13)   */
  -120,   /* a(n-14)   */
    8,    /* a(n-15)   */
   -45,   /* a(n-16)   */
    1,    /* a(n-17)   */
  -10,    /* a(n-18)   */
    0,    /* a(n-19)   */
   -1     /* a(n-20)   */
};

seq_t a(idx_t n)
{
    /* Initialize cache on first call. */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failed */
        for (size_t i = 0; i < 20; ++i)
            cache[i] = base[i];
        cache_len = 20;
    }

    /* Grow the cache array if necessary. */
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= (idx_t)new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any terms beyond what we have cached. */
    if ((size_t)n >= cache_len) {
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            if (k < 20) {            /* base values already stored. */
                continue;
            }
            seq_t val = 0;
            for (int off = 2; off <= 20; ++off) {
                int c = coeff[off];
                if (c) val += (seq_t)c * cache[k - off];
            }
            cache[k] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}

/*
 * A219837  Expansion of
 *          x^4*(2-10*x+18*x^2-7*x^3-21*x^4+25*x^5-x^6)/((1-x)^3*(1-2*x)^6).
 * https://oeis.org/A219837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 47, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 15*a(n-1) - 99*a(n-2) + 377*a(n-3) - 912*a(n-4)
 * + 1452*a(n-5) - 1520*a(n-6) + 1008*a(n-7) - 384*a(n-8) + 64*a(n-9)
 * + b_n
 * where b_n is the coefficient of x^n in x^4*(2-10x+18x^2-7x^3-21x^4+25x^5-x^6),
 * i.e. b_0..b_3=0, b_4=2, b_5=-10, b_6=18, b_7=-7, b_8=-21, b_9=25, b_10=-1,
 * and b_n=0 for n>10.  a(0)=a(1)=a(2)=a(3)=0.
 */
#include <stdlib.h>

static const int B[11] = {0, 0, 0, 0, 2, -10, 18, -7, -21, 25, -1};

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* current allocated capacity */
    static size_t last_computed = 0;/* next index to compute */

    /* Ensure capacity for n+1 indices */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failed, return 0 as fallback */
            return (seq_t)0;
        }
        /* initialise new entries to 0 */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = newcap;
    }

    /* compute values up to n */
    for (size_t i = last_computed; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = 0;
            continue;
        }

        seq_t pos = 0, neg = 0;

        /* positive coefficient terms */
        if (i >= 1) pos += cache[i-1] * 15;
        if (i >= 3) pos += cache[i-3] * 377;
        if (i >= 5) pos += cache[i-5] * 1452;
        if (i >= 7) pos += cache[i-7] * 1008;
        if (i >= 9) pos += cache[i-9] * 64;

        /* negative coefficient terms */
        if (i >= 2) neg += cache[i-2] * 99;
        if (i >= 4) neg += cache[i-4] * 912;
        if (i >= 6) neg += cache[i-6] * 1520;
        if (i >= 8) neg += cache[i-8] * 384;

        seq_t val = pos;

        /* add the numerator coefficient b_n */
        int bcoeff = (i <= 10) ? B[i] : 0;
        if (bcoeff >= 0)
            val += (seq_t)bcoeff;
        else
            val -= (seq_t)(-bcoeff);

        /* subtract negative coefficient contribution */
        val -= neg;

        cache[i] = val;
    }
    last_computed = n + 1;
    return cache[n];
}

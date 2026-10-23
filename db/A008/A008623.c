/*
 * A008623  Molien series of 4-dimensional representation of SL(2,7).
 * https://oeis.org/A008623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (x^14 + x^10 + x^9 + x^8 + x^6 + x^5 + x^4 + 1) /
 *            ((1-x^2)*(1-x^3)*(1-x^4)*(1-x^7))
 * Range:     u8 n <= 30, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6)-a(n-10)
                     -a(n-11)+a(n-12)+a(n-13)+a(n-14)-a(n-16) + p(n), */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Denominator coefficients Q(x) = 1 - x^2 - x^3 - x^4 + x^5 + x^6
   - still -x^10 -x^11 + x^12 + x^13 + x^14 + x^16   (values above)
   Q[0] is always 1.                                          */
static const int Q[17] = {
    1, 0, -1, -1, -1, 1, 1, 0, 0, 0, 1, 1, -1, -1, -1, 0, 1
};

/* Numerator 1 + x^4 + x^5 + x^6 + x^8 + x^9 + x^10 + x^14. */
static const bool P_present[15] = {
    1, /* 0 */
    0, /* 1 */
    0, /* 2 */
    0, /* 3 */
    1, /* 4 */
    1, /* 5 */
    1, /* 6 */
    0, /* 7 */
    1, /* 8 */
    1, /* 9 */
    1, /*10 */
    0, /*11 */
    0, /*12 */
    0, /*13 */
    1  /*14 */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Grow cache and compute missing terms up to n. */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;

        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t val = 0;
            /* Numerator contribution */
            if (i <= 14 && P_present[i])
                val = 1;
            /* Recurrence from denominator */
            for (int k = 1; k <= 16 && (size_t)k <= i; ++k) {
                int qk = Q[k];
                if (qk == 1)         /* subtract Q[k]*a(n-k) = a(n-k) */
                    val -= cache[i - k];
                else if (qk == -1)   /* subtract(-1)*a = +a */
                    val += cache[i - k];
            }
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}

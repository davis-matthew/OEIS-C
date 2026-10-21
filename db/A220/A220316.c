/*
 * A220316  Equals two maps: number of nX2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly two of their
 *          king-move neighbors in a random 0..2 nX2 array.
 * https://oeis.org/A220316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+6*a(n-2)-22*a(n-3)-15*a(n-4)+10*a(n-5)+56*a(n-6)
 *            *
 *            +46*a(n-7)-112*a(n-8)-84*a(n-9)-67*a(n-10)+358*a(n-11)+277*a(n-12)
 *            * -168*a(n-13)-638*a(n-14)-162*a(n-15)+664*a(n-16)+408*a(n-17) *
 *            +180*a(n-18)-8*a(n-19)+76*a(n-20)-264*a(n-21)+96*a(n-22) *
 *            Initial values: a(1)=1,a(2)=5,a(3)=24,a...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static table of the first 22 terms (index 0 -> a(1)) */
    static const seq_t init[22] = {
        1ULL,     5ULL,     24ULL,    89ULL,   389ULL,   1566ULL,
        6401ULL,  25704ULL, 103047ULL,411453ULL,1640962ULL,6538036ULL,
        26037222ULL,103664093ULL,412658625ULL,1642555969ULL,6537717638ULL,
        26020883563ULL,103564418265ULL,412188609650ULL,1640510914951ULL,
        6529219819790ULL
    };

    /* Coefficients for the recurrence, corresponding to a(n-1) ... a(n-22) */
    static const int coeff[22] = {
          4,    6,   -22,  -15,   10,   56,   46,  -112,  -84,  -67,
        358,  277, -168, -638, -162, 664, 408, 180,   -8,   76,
       -264,   96
    };

    /* Cache for computed values */
    static seq_t *cache = NULL;   /* dynamically allocated array a(1)... */
    static size_t  cap   = 0;      /* current capacity of cache */
    static size_t  used  = 0;      /* number of terms actually stored */

    /* Return an initial value directly if it is requested. */
    if (n <= 22) {
        return init[(size_t)(n - 1)];
    }

    /* Ensure the cache can store up to n terms. */
    if (cap < (size_t)n) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* If we already have the requested term, bail out. */
    if (used >= (size_t)n) {
        return cache[(size_t)n - 1];
    }

    /* Initialize the cache with the known first 22 terms. */
    if (used < 22) {
        for (size_t i = 0; i < 22; ++i) {
            cache[i] = init[i];
        }
        used = 22;
    }

    /* Compute successive terms using the recurrence. */
    for (size_t i = used; i < (size_t)n; ++i) {          /* i --> index of a(i+1) */
        seq_t positive = 0;
        seq_t negative = 0;

        for (int j = 0; j < 22; ++j) { /* j corresponds to coefficient a(n-(j+1)) */
            /* cache index for a(n-(j+1)) when n = i+1 :
             * a((i+1)-(j+1)) = a(i-j) -> cache[i-j-1]   */
            size_t idx = i - 1 - (size_t)j;
            seq_t val = cache[idx];
            if (coeff[j] >= 0) {
                positive += (seq_t)coeff[j] * val;
            } else {
                negative += (seq_t)(-coeff[j]) * val;
            }
        }
        /* Recurrence guarantees positive >= negative for all n >= 23. */
        cache[i] = positive - negative;
    }

    used = (size_t)n;
    return cache[(size_t)n - 1];
}

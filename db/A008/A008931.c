/*
 * A008931  Expansion of (2/(1+sqrt(1-36*x)))^(1/3).
 * https://oeis.org/A008931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(18*n^2-21*n+5)*a(n-1)/(n*(3*n+1)) Recurrence: a(0) = 1
 *            a(n) = a(n-1) * 6*(18*n^2-21*n+5) / (n*(3*n+1)) (n>=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *CACHE = NULL;   /* cache of computed terms */
static size_t CAP    = 0;     /* current cache size */
static idx_t  LAST   = 0;     /* highest index computed */

seq_t a(idx_t n)
{
    /* Ensure cache exists and has a(0)=1 */
    if (CAP == 0) {
        CACHE = (seq_t*)malloc(sizeof(seq_t));
        if (!CACHE) return (seq_t)0;
        CAP   = 1;
        CACHE[0] = (seq_t)1;
    }

    if (n == 0) return CACHE[0];

    /* Grow cache if needed */
    if ((size_t)n >= CAP) {
        size_t newcap = CAP ? CAP : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(CACHE, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        CACHE = tmp;
        CAP   = newcap;
    }

    /* Compute missing values up to n */
    if (n > LAST) {
        for (idx_t i = LAST + 1; i <= n; ++i) {
            seq_t prev = CACHE[i - 1];

            /* numerator and denominator of the fraction in the recurrence */
            seq_t num = (seq_t)6 *
                        (((seq_t)18 * i * i) - ((seq_t)21 * i) + (seq_t)5);
            seq_t den = (seq_t)i * (((seq_t)3 * i) + (seq_t)1);

            /* Reduce with gcd to keep intermediate numbers small */
            seq_t g1 = gcd(prev, den);
            seq_t prev_red = prev / g1;
            seq_t den_red  = den  / g1;

            seq_t g2 = gcd(num, den_red);
            seq_t num_red = num / g2;
            seq_t den_red2 = den_red / g2;

            seq_t prod = prev_red * num_red;
            seq_t val  = prod / den_red2;

            CACHE[i] = val;
        }
        LAST = n;
    }

    return CACHE[n];
}

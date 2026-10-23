/*
 * A016063  Inverse of 2054th cyclotomic polynomial.
 * https://oeis.org/A016063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ2054(x)
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define P 2054   /* period length */

/* cache holds one full period of the sequence */
static seq_t cache[P];
static bool initialized = false;

/* compute the periodic sequence once */
static void compute_cache(void)
{
    if (initialized)
        return;

    /* initialise a(0)=1, all others 0 */
    for (size_t i = 0; i < P; ++i)
        cache[i] = 0;
    cache[0] = 1;

    /* divisors of 2054 together with the sign of the exponent e_d
       as derived from e_d = -μ(2054/d).  e = +1 -> multiply by (1 - x^d)
       e = -1 -> multiply by 1/(1 - x^d).               */
    static const struct { unsigned int d; int e; } terms[] = {
        { 1,    +1 },
        { 2,    -1 },
        { 13,   -1 },
        { 26,   +1 },
        { 79,   -1 },
        { 158,  +1 },
        { 1027, +1 },
        { 2054, -1 }
    };
    const size_t nt = sizeof terms / sizeof terms[0];
    seq_t tmp[P];

    for (size_t t = 0; t < nt; ++t) {
        unsigned int d = terms[t].d;
        int e = terms[t].e;

        if (e == +1) {   /* multiply by (1 - x^d) */
            for (size_t k = 0; k < P; ++k) {
                if (k >= d)
                    tmp[k] = cache[k] - cache[k - d];
                else
                    tmp[k] = cache[k];
            }
        } else {         /* multiply by 1/(1 - x^d) */
            for (size_t k = 0; k < P; ++k) {
                if (k >= d)
                    tmp[k] = cache[k] + tmp[k - d];
                else
                    tmp[k] = cache[k];
            }
        }

        /* copy new values back into cache for next iteration */
        for (size_t k = 0; k < P; ++k)
            cache[k] = tmp[k];
    }

    initialized = true;
}

/* public function required by the specification */
seq_t a(idx_t n)
{
    compute_cache();
    return cache[(size_t)(n % P)];
}

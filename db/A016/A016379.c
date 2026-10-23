/*
 * A016379  310th cyclotomic polynomial.
 * https://oeis.org/A016379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 310th cyclotomic polynomial
 *            Φ_310(x), computed via the Möbius product formula Φ_n(x) =
 *            ∏_{d|n} (x^d‑1)^{μ(n/d)}
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"

#define MAX_POLY 500   /* Maximum degree needed when multiplying (350 is enough) */

typedef uint64_t idx_t;   /* provided by the build */

/* Möbius function μ(m) for unsigned 64‑bit integers */
static int mu_uint64(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > 1) return 0;
    return (cnt % 2 == 0) ? 1 : -1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static storage for the coefficients of Φ_310(x) */
    static seq_t coeff[MAX_POLY];
    static size_t coeff_len = 0;
    static bool ready = false;

    if (!ready) {
        /* Compute Φ_310(x) via Möbius product */
        const uint64_t N = 310u;

        /* Gather all divisors of N */
        uint64_t divs[64];
        size_t divcnt = 0;
        for (uint64_t i = 1; i * i <= N; ++i)
            if (N % i == 0) {
                divs[divcnt++] = i;
                if (i * i != N)
                    divs[divcnt++] = N / i;
            }

        /* Separate divisors into those with μ(n/d)=+1 and μ(n/d)=-1 */
        uint64_t pos[64], neg[64];
        size_t poscnt = 0, negcnt = 0;
        for (size_t k = 0; k < divcnt; ++k) {
            uint64_t d = divs[k];
            int mu = mu_uint64(N / d);
            if (mu == 1)
                pos[poscnt++] = d;
            else if (mu == -1)
                neg[negcnt++] = d;
        }

        /* Polynomial representation: poly[deg] is coefficient of x^deg */
        seq_t poly[MAX_POLY] = {0};
        size_t deg = 0;
        poly[0] = (seq_t)1;

        /* Multiply by all factors with +1 exponent */
        for (size_t k = 0; k < poscnt; ++k) {
            uint64_t d = pos[k];
            size_t newdeg = deg + (size_t)d;
            seq_t temp[MAX_POLY] = {0};
            for (size_t i = 0; i <= deg; ++i) {
                temp[i]          -= poly[i];      /* subtract old term */
                temp[i + d]      += poly[i];      /* add shifted term */
            }
            for (size_t i = 0; i <= newdeg; ++i)
                poly[i] = temp[i];
            deg = newdeg;
        }

        /* Divide by all factors with -1 exponent */
        for (size_t k = 0; k < negcnt; ++k) {
            uint64_t d = neg[k];
            size_t mdeg = deg;
            size_t newdeg = mdeg - (size_t)d;

            seq_t rem[MAX_POLY];
            for (size_t i = 0; i <= mdeg; ++i)
                rem[i] = poly[i];

            seq_t q[MAX_POLY] = {0};

            for (int i = (int)mdeg; i >= (int)d; --i) {
                seq_t coeff = rem[i];
                if (coeff != 0) {
                    q[i - (int)d] = coeff;
                    rem[i]          -= coeff;     /* subtract coeff * 1 */
                    rem[i - (int)d] += coeff;     /* add coeff * 1 because subtracting -1 */
                }
            }
            for (size_t i = 0; i <= newdeg; ++i)
                poly[i] = q[i];
            deg = newdeg;
        }

        /* Store coefficients (degree should be φ(310)=120) */
        coeff_len = deg + 1;
        for (size_t i = 0; i < coeff_len; ++i)
            coeff[i] = poly[i];

        ready = true;
    }

    if (n < coeff_len)
        return coeff[(size_t)n];
    return (seq_t)0;
}

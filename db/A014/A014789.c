/*
 * A014789  Inverse of 780th cyclotomic polynomial.
 * https://oeis.org/A014789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = coefficient of x^n in the power series expansion
   of the reciprocal of the 780th cyclotomic polynomial Φ_780(x).
   The sequence is periodic with period 780. */
#include <stdbool.h>
#include <stddef.h>

static int mobius(unsigned long long x) {
    int cnt = 0;
    for (unsigned long long p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
                if (e > 1) return 0;
            }
            ++cnt;
        }
    }
    if (x > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n) {
    static seq_t a_cache[780];
    static bool ready = false;
    if (!ready) {
        const unsigned long long m = 780ULL;
        /* Generate list of divisors of 780 */
        const unsigned long long divs[] = {1,2,3,4,5,6,10,12,13,15,20,26,30,39,52,60,65,78,130,156,195,260,390,780};
        const unsigned div_cnt = sizeof(divs) / sizeof(divs[0]);

        /* Φ_780(x) degree */
        const unsigned deg = 192; /* ϕ(780) = 192 */
        int16_t coeff[193] = {0};
        int16_t tmp[193] = {0};
        coeff[0] = 1; /* initialise polynomial 1 */

        /* First multiply by factors with μ=1 */
        for (unsigned i = 0; i < div_cnt; ++i) {
            unsigned d = (unsigned)divs[i];
            unsigned mu = mobius(m / d);
            if (mu != 1) continue;
            for (unsigned j = 0; j <= deg; ++j) {
                int64_t val = 0;
                if (j >= d) val += coeff[j - d];
                val -= coeff[j];
                tmp[j] = (int16_t)val;
            }
            for (unsigned j = 0; j <= deg; ++j) coeff[j] = tmp[j];
        }
        /* Then divide by factors with μ=-1 */
        for (unsigned i = 0; i < div_cnt; ++i) {
            unsigned d = (unsigned)divs[i];
            unsigned mu = mobius(m / d);
            if (mu != -1) continue;
            for (unsigned j = 0; j <= deg; ++j) {
                int64_t prev = (j >= d) ? tmp[j - d] : 0;
                int64_t val  = prev - coeff[j];
                tmp[j] = (int16_t)val;
            }
            for (unsigned j = 0; j <= deg; ++j) coeff[j] = tmp[j];
        }

        /* Convert coefficients to seq_t */
        seq_t phi_coeff[193];
        for (unsigned i = 0; i <= deg; ++i) phi_coeff[i] = (seq_t)coeff[i];

        /* Compute the series a(n) for n=0..779 */
        a_cache[0] = (seq_t)1;
        for (unsigned i = 1; i < 780; ++i) {
            seq_t s = (seq_t)0;
            unsigned up = (i <= deg) ? i : deg;
            for (unsigned k = 1; k <= up; ++k) {
                s += phi_coeff[k] * a_cache[i - k];
            }
            a_cache[i] = -s;
        }
        ready = true;
    }

    idx_t idx = n % 780ULL;
    return a_cache[idx];
}

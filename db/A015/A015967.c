/*
 * A015967  Inverse of 1958th cyclotomic polynomial.
 * https://oeis.org/A015967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_1958(x). The sequence is periodic with period 1958
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PERIOD 1958

static seq_t coeff[PERIOD];
static bool initialized = false;

static int mu_of_uint64(uint64_t m) {
    int cnt = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;   /* square factor */
            ++cnt;
        }
    }
    if (m > 1) ++cnt;                    /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;           /* (-1)^cnt  */
}

static void init_coefficients(void) {
    /* compute all divisors of PERIOD */
    size_t divs[64];
    size_t div_cnt = 0;
    for (size_t i = 1; i * i <= PERIOD; ++i) {
        if (PERIOD % i == 0) {
            divs[div_cnt++] = i;
            if (i != PERIOD / i) divs[div_cnt++] = PERIOD / i;
        }
    }

    /* start with 1 + 0x + 0x^2 + ...  */
    for (size_t i = 0; i < PERIOD; ++i) coeff[i] = 0;
    coeff[0] = 1;

    /* apply the product  ∏_{d|n} (1 - x^d)^{-μ(n/d)}   */
    for (size_t di = 0; di < div_cnt; ++di) {
        size_t d = divs[di];
        int mu = mu_of_uint64(PERIOD / d);

        if (mu == 1) {                     /* multiply by (1 - x^d)^{-1} */
            seq_t tmp[PERIOD];
            for (size_t i = 0; i < PERIOD; ++i) {
                seq_t sum = 0;
                for (size_t k = 0; d * k <= i; ++k) {
                    sum += coeff[i - d * k];
                }
                tmp[i] = sum;
            }
            for (size_t i = 0; i < PERIOD; ++i) coeff[i] = tmp[i];
        } else if (mu == -1) {              /* multiply by (1 - x^d) */
            for (size_t i = PERIOD; i-- > 0; ) {
                if (i >= d) coeff[i] -= coeff[i - d];
            }
        }                                     /* mu == 0: nothing to do   */
    }

    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_coefficients();
    size_t idx = (size_t)(n % PERIOD);
    return coeff[idx];
}

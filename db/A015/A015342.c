/*
 * A015342  Inverse of 1333rd cyclotomic polynomial.
 * https://oeis.org/A015342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Periodic series a(n) = -∑_{k=1}^{1260} c_k · a(n-k), where c_k
 *            are the coefficients of the cyclotomic polynomial Φ₁₃₃₃(x). The
 *            sequence has period 1333 and a(0)=1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define PERIOD   1333
#define DEG_Q    1260
#define DEG_H    74
#define DEG_N    1334

static bool ready = false;
static seq_t seq_cache[PERIOD];
static seq_t coeff[DEG_Q + 1];

static void init(void)
{
    /* temporary storage for the polynomials, using 32‑bit ints */
    int32_t ncoeff[DEG_N + 1] = {0};
    int32_t h[DEG_H + 1]   = {0};
    int32_t q[DEG_Q + 1]   = {0};

    /* N(x) = (x^1334 – x^1333 – x + 1) */
    ncoeff[0]   =  1;
    ncoeff[1]   = -1;
    ncoeff[1333] = -1;
    ncoeff[1334] =  1;

    /* H(x) = (x^43 – 1)(x^31 – 1) = x^74 – x^43 – x^31 + 1 */
    h[0]  =  1;
    h[31] = -1;
    h[43] = -1;
    h[74] =  1;

    /* Polynomial division: N / H  →  Φ₁₃₃₃ */
    for (int i = DEG_N; i >= DEG_H; --i) {
        int32_t coef = ncoeff[i];
        if (coef != 0) {
            int pos = i - DEG_H;
            q[pos] = coef;                         /* leading coefficient of H is 1 */
            for (int j = 0; j <= DEG_H; ++j) {
                ncoeff[pos + j] -= coef * h[j];
            }
        }
    }

    /* Copy the quotient coefficients into the global array */
    for (int i = 0; i <= DEG_Q; ++i) {
        coeff[i] = (seq_t)q[i];
    }

    /* Generate the periodic sequence of length PERIOD */
    seq_cache[0] = (seq_t)1;
    for (int m = 1; m < PERIOD; ++m) {
        seq_t sum = 0;
        int lim = (m <= DEG_Q) ? m : DEG_Q;
        for (int i = 1; i <= lim; ++i) {
            sum += coeff[i] * seq_cache[m - i];
        }
        seq_cache[m] = -sum;
    }

    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) init();
    return seq_cache[(size_t)(n % PERIOD)];
}

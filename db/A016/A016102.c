/*
 * A016102  Inverse of 2093rd cyclotomic polynomial.
 * https://oeis.org/A016102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1 / Φ₍₂₀₉₃₎(x); the sequence is periodic with period 2093. The
 *            coefficients are computed as a convolution of two short integer
 *            sequences: the coefficients of (x-1)(x⁹¹-1)(x¹⁶¹-1)(x²⁹⁹-1) and
 *            the number of representations of an intege...
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define PERIOD 2093
#define MAX_N   2092

static seq_t period[PERIOD];
static bool initialized = false;

static void init(void)
{
    seq_t m[PERIOD];            /* number of representations (i,j,k)   */
    seq_t Q[553];               /* coefficients of (x-1)(x⁹¹-1)(x¹⁶¹-1)(x²⁹⁹-1) */

    /* zero the auxiliary arrays */
    memset(m, 0, sizeof(m));
    memset(Q, 0, sizeof(Q));

    /* ----- compute m[e] for 0 ≤ e ≤ 2092 ----- */
    idx_t max_i = MAX_N / 7;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t i7 = i * 7;
        idx_t max_j = (MAX_N - i7) / 13;
        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t j13 = j * 13;
            idx_t remaining = MAX_N - i7 - j13;
            idx_t max_k = remaining / 23;
            for (idx_t k = 0; k <= max_k; ++k) {
                idx_t e = i7 + j13 + k * 23;
                m[e] += 1;
            }
        }
    }

    /* ----- compute Q[t] for 0 ≤ t ≤ 552 ----- */
    /*   Q[t] = coefficient of x^t in (x-1)(x⁹¹-1)(x¹⁶¹-1)(x²⁹⁹-1)   */
    for (int subset = 0; subset < 16; ++subset) {
        idx_t t = 0;
        int negatives = 0;          /* number of chosen -1 terms */

        /* factor (x-1) */
        if ((subset & 1) == 0) {
            negatives++;            /* choose -1 */
        } else {
            t += 1;                 /* choose x^1 */
        }
        /* factor (x⁹¹-1) */
        if ((subset & 2) == 0) {
            negatives++;
        } else {
            t += 91;
        }
        /* factor (x¹⁶¹-1) */
        if ((subset & 4) == 0) {
            negatives++;
        } else {
            t += 161;
        }
        /* factor (x²⁹⁹-1) */
        if ((subset & 8) == 0) {
            negatives++;
        } else {
            t += 299;
        }

        int sign = (negatives % 2 == 0) ? 1 : -1;
        Q[t] = (seq_t)sign;        /* coefficient (+1 or -1) */
    }

    /* ----- compute the period coefficients a[0..2092] ----- */
    for (idx_t n = 0; n < PERIOD; ++n) {
        seq_t sum = 0;
        for (idx_t t = 0; t <= n && t < 553; ++t) {
            if (Q[t] != 0) {
                sum += Q[t] * m[n - t];
            }
        }
        period[n] = sum;           /* a[n] = Σ Q[t] * m[n-t]           */
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }
    return period[n % PERIOD];
}

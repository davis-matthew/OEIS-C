/*
 * A015726  Inverse of 1717th cyclotomic polynomial.
 * https://oeis.org/A015726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power * series expansion
 *            of 1/Φ1717(x), periodic with period 1717
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Exact coefficients of Φ1717(x) (degree 1600). */
static void build_phi(seq_t phi[1601]) {
    const seq_t ZERO = (seq_t)0;
    seq_t P[1719]; /* numerator: x^1718 - x^1717 - x + 1 */
    seq_t Q[119];  /* denominator: x^118 - x^101 - x^17 + 1 */

    /* init to zero */
    for (int i = 0; i < 1719; ++i) P[i] = ZERO;
    for (int i = 0; i < 119;  ++i) Q[i] = ZERO;

    /* build numerator */
    P[0]    = (seq_t)1;
    P[1]    = (seq_t)-1;
    P[1717] = (seq_t)-1;
    P[1718] = (seq_t)1;

    /* build denominator */
    Q[0]   = (seq_t)1;
    Q[17]  = (seq_t)-1;
    Q[101] = (seq_t)-1;
    Q[118] = (seq_t)1;

    /* exact polynomial long division: P / Q */
    for (int i = 1718; i >= 118; --i) {
        seq_t coeff = P[i];
        int qidx = i - 118;        /* quotient index in phi */
        phi[qidx] = coeff;        /* store quotient coefficient */
        if (coeff != ZERO) {
            for (int j = 0; j <= 118; ++j) {
                P[qidx + j] -= coeff * Q[j];
            }
        }
    }
    /* the leading part of P is now zero because division is exact */
}

/* Build the periodic array of a(n) for 0 ≤ n < 1717. */
static void build_period(seq_t period[1717]) {
    static seq_t phi[1601];
    build_phi(phi);

    period[0] = (seq_t)1;
    for (int k = 1; k < 1717; ++k) {
        seq_t sum = (seq_t)0;
        int maxk = (k < 1601) ? k : 1600;
        for (int j = 1; j <= maxk; ++j) {
            sum += phi[j] * period[k - j];
        }
        period[k] = -sum;
    }
}

seq_t a(idx_t n) {
    static seq_t period[1717];
    static bool initialized = false;
    if (!initialized) {
        build_period(period);
        initialized = true;
    }
    return period[n % 1717];
}

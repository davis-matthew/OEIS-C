/*
 * A015780  Inverse of 1771st cyclotomic polynomial.
 * https://oeis.org/A015780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Inverse of 1771st cyclotomic polynomial
    a(n) is the coefficient of x^n in the power‑series expansion of
    1 / Φ_1771(x).  It is periodic with period 1771.  This implementation
    generates the coefficients by first computing the reciprocal of
    (1‑x^7)(1‑x^11)(1‑x^23) up to one period, then multiplying by
    (1‑x)(1‑x^77)(1‑x^161)(1‑x^253).  All intermediate arithmetic uses
    64‑bit signed integers to avoid overflow for any build size of
    seq_t. */

#include <stdbool.h>

enum { PERIOD = 1771 };

static bool initialized = false;
static seq_t A[PERIOD];          /* cached final coefficients */

/*  Compute the coefficients once, in a 64‑bit context */
static void init_sequence(void)
{
    long long B[PERIOD];
    long long sum;

    /*  B[t] = number of representations of t by 7i+11j+23k  */
    for (int i = 0; i < PERIOD; ++i) B[i] = 0;
    B[0] = 1;
    int coins[3] = {7, 11, 23};
    for (int c = 0; c < 3; ++c) {
        int coin = coins[c];
        for (int t = coin; t < PERIOD; ++t)
            B[t] += B[t - coin];
    }

    /*  A[t] = coefficient of 1/Φ_1771(x) modulo 1771, using
         (1‑x)(1‑x^77)(1‑x^161)(1‑x^253).  */
    for (int t = 0; t < PERIOD; ++t) {
        sum = 0;
        /* shift 0 */
        sum += B[t];
        /* -1 shifts */
        if (t >= 1)   sum -= B[t - 1];
        if (t >= 77)  sum -= B[t - 77];
        if (t >= 161) sum -= B[t - 161];
        if (t >= 253) sum -= B[t - 253];
        /* +2 shifts */
        if (t >= 78)  sum += B[t - 78];
        if (t >= 162) sum += B[t - 162];
        if (t >= 238) sum += B[t - 238];
        if (t >= 254) sum += B[t - 254];
        if (t >= 330) sum += B[t - 330];
        if (t >= 414) sum += B[t - 414];
        /* -3 shifts */
        if (t >= 239) sum -= B[t - 239];
        if (t >= 331) sum -= B[t - 331];
        if (t >= 415) sum -= B[t - 415];
        if (t >= 491) sum -= B[t - 491];
        /* +4 shift */
        if (t >= 492) sum += B[t - 492];

        /* Store into cache */
        A[t] = (seq_t)sum;  /* cast to seq_t; final values fit */
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    idx_t pos = n % PERIOD;   /* period 1771 */
    return A[pos];
}

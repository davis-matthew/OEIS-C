/*
 * A016086  Inverse of 2077th cyclotomic polynomial.
 * https://oeis.org/A016086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series of 1/Φ₂₀₇₇(x). The
 *            coefficients form a periodic sequence of period 2077
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t cache[2077];
    static bool initialized = false;

    if (!initialized) {
        /* Step 1: compute Φ₂₀₇₇(x) = (x²⁰⁷⁷−1) / (
           (x−1)·(1+x+⋯+x³⁰)·(1+x+⋯+x⁶⁶) )   */
        const int degN  = 2077;                       /* degree of x²⁰⁷⁷−1        */
        const int degP1 = 1;                          /* degree of (x−1)          */
        const int degP2 = 30;                         /* degree of 1+⋯+x³⁰        */
        const int degP3 = 66;                         /* degree of 1+⋯+x⁶⁶        */
        const int degD  = degP1 + degP2 + degP3;      /* degree of D              */
        const int degQ  = degN - degD;                /* degree of Φ₂₀₇₇          */
        const int sizeQ = degQ + 1;                   /* number of coefficients   */
        const int sizeD = degD + 1;                   /* array for D              */

        /* temporary arrays */
        seq_t D[sizeD];
        seq_t rem[degN + 1];
        seq_t Phi[sizeQ];
        seq_t tempP2[degP1 + degP2 + 1];
        seq_t tempP3[degD + 1];

        /* build D = (x−1)·(1+⋯+x³⁰)·(1+⋯+x⁶⁶) */

        /* coefficients of the small polynomials */
        seq_t P1c[2] = { -1, 1 };            /* (x−1)         */
        seq_t P2c[31];
        for (int k = 0; k <= degP2; ++k) P2c[k] = 1;   /* 1+⋯+x³⁰      */
        seq_t P3c[67];
        for (int k = 0; k <= degP3; ++k) P3c[k] = 1;   /* 1+⋯+x⁶⁶      */

        /* multiply (x−1) with (1+⋯+x³⁰)  */
        for (int i = 0; i <= degP1 + degP2; ++i) tempP2[i] = 0;
        for (int i = 0; i <= degP1; ++i)
            for (int j = 0; j <= degP2; ++j)
                tempP2[i + j] += P1c[i] * P2c[j];

        /* multiply the result with (1+⋯+x⁶⁶) */
        for (int i = 0; i <= degD; ++i) tempP3[i] = 0;
        for (int i = 0; i <= degP1 + degP2; ++i)
            for (int j = 0; j <= degP3; ++j)
                tempP3[i + j] += tempP2[i] * P3c[j];

        for (int i = 0; i <= degD; ++i) D[i] = tempP3[i];

        /* Step 2: divide (x²⁰⁷⁷−1) by D to obtain Φ₂₀₇₇(x) */
        for (int i = 0; i <= degN; ++i) rem[i] = 0;
        rem[0] = -1;                 /* −1 at x⁰               */
        rem[degN] = 1;               /* +1 at x²⁰⁷⁷            */

        for (int i = degQ; i >= 0; --i) {
            seq_t factor = rem[i + degD];    /* leading coeff of D is 1 */
            Phi[i] = factor;
            if (factor != 0) {
                for (int j = 0; j <= degD; ++j)
                    rem[i + j] -= factor * D[j];
            }
        }

        /* Step 3: compute the inverse series S(x) such that
           Φ₂₀₇₇(x)·S(x) ≡ 1 (mod x²⁰⁷⁷) */
        seq_t S[2077];
        S[0] = 1;
        for (int m = 1; m < 2077; ++m) {
            seq_t sum = 0;
            int maxk = (m < sizeQ) ? m : sizeQ - 1;
            for (int k = 1; k <= maxk; ++k)
                sum += Phi[k] * S[m - k];
            S[m] = -sum;
        }

        /* store the period in the cache */
        for (int i = 0; i < 2077; ++i)
            cache[i] = S[i];

        initialized = true;
    }

    /* return periodic value */
    return cache[(size_t)(n % 2077)];
}

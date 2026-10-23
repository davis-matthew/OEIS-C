/*
 * A016412  442nd cyclotomic polynomial.
 * https://oeis.org/A016412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 442nd cyclotomic polynomial
 *            Φ_442(x)
 * Range:     i8 n <= 192, i16 n <= 192, i32 n <= 192, i64 n <= 192,
 *            i128 n <= 192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define DEG_Q 192
#define MAX_N 500

seq_t a(idx_t n)
{
    static seq_t coeff[DEG_Q + 1];
    static int initialized = 0;
    if (!initialized) {
        /* Signed Möbius contributions (μ(d) = +1) */
        const int ksN[] = { 442, 17, 13, 2 };
        /* Unsigned Möbius contributions (μ(d) = -1) */
        const int ksD[] = { 221, 34, 26, 1 };
        const size_t NUM_KS_N = sizeof ksN / sizeof ksN[0];
        const size_t NUM_KS_D = sizeof ksD / sizeof ksD[0];

        seq_t N[MAX_N];
        seq_t D[MAX_N];
        seq_t temp[MAX_N];
        size_t degN = 0, degD = 0;

        for (size_t i = 0; i < MAX_N; ++i) {
            N[i] = 0;
            D[i] = 0;
        }

        /* Build N(x) = ∏ (x^k − 1) for ·ksN */
        N[0] = 1;
        degN = 0;
        for (size_t idx = 0; idx < NUM_KS_N; ++idx) {
            int k = ksN[idx];
            for (size_t i = 0; i < MAX_N; ++i) temp[i] = 0;
            for (size_t i = 0; i <= degN; ++i) {
                temp[i] -= N[i];
                temp[i + k] += N[i];
            }
            degN += k;
            for (size_t i = 0; i <= degN; ++i) N[i] = temp[i];
        }

        /* Build D(x) = ∏ (x^k − 1) for ·ksD */
        D[0] = 1;
        degD = 0;
        for (size_t idx = 0; idx < NUM_KS_D; ++idx) {
            int k = ksD[idx];
            for (size_t i = 0; i < MAX_N; ++i) temp[i] = 0;
            for (size_t i = 0; i <= degD; ++i) {
                temp[i] -= D[i];
                temp[i + k] += D[i];
            }
            degD += k;
            for (size_t i = 0; i <= degD; ++i) D[i] = temp[i];
        }

        /* Long division N / D to obtain Φ_442(x) = Q(x) */
        seq_t Q[DEG_Q + 1];
        for (size_t i = 0; i <= DEG_Q; ++i) Q[i] = 0;

        for (size_t i = degN; i >= degD; --i) {
            seq_t q = N[i];
            Q[i - degD] = q;
            for (size_t j = 0; j <= degD; ++j) {
                N[i - degD + j] -= q * D[j];
            }
        }

        for (size_t i = 0; i <= DEG_Q; ++i) coeff[i] = Q[i];
        initialized = 1;
    }

    if (n > (idx_t)DEG_Q)
        return 0;
    return coeff[(size_t)n];
}

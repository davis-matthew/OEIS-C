/*
 * A002130  Generalized sum of divisors function.
 * https://oeis.org/A002130
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f.: (t(1)^2-t(2))/2 where t(1) = Sum_{n>=1} x^n/(1+x^n)^2,
 *            t(2) = Sum_{n>=1} x^(2n)/(1+x^n)^4
 * Range:     i8 n <= 8, i16 n <= 51, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    seq_t *t1 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *t2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *t1_sq = (seq_t *)calloc(n + 1, sizeof(seq_t));
    
    // Compute t(1) = Sum_{m>=1} x^m/(1+x^m)^2
    // Expansion: 1/(1+y)^2 = Sum_{k>=0} (-1)^k(k+1)y^k
    for (idx_t m = 1; m <= n; m++) {
        for (idx_t k = 0; m * (k + 1) <= n; k++) {
            idx_t power = m * (k + 1);
            seq_t sign = (k & 1) ? -(seq_t)1 : (seq_t)1;
            t1[power] += sign * (seq_t)(k + 1);
        }
    }
    
    // Compute t(2) = Sum_{m>=1} x^(2m)/(1+x^m)^4
    // Expansion: 1/(1+y)^4 = Sum_{k>=0} (-1)^k*C(k+3,3)y^k
    for (idx_t m = 1; m <= n; m++) {
        for (idx_t k = 0; m * (k + 2) <= n; k++) {
            idx_t power = m * (k + 2);
            seq_t sign = (k & 1) ? -(seq_t)1 : (seq_t)1;
            seq_t binom = (seq_t)(k + 1) * (seq_t)(k + 2) * (seq_t)(k + 3) / 6;
            t2[power] += sign * binom;
        }
    }
    
    // Compute t(1)^2
    for (idx_t i = 0; i <= n; i++) {
        for (idx_t j = 0; i + j <= n; j++) {
            t1_sq[i + j] += t1[i] * t1[j];
        }
    }
    
    // Extract coefficient of x^n from (t(1)^2 - t(2))/2
    seq_t result = (t1_sq[n] - t2[n]) / 2;
    
    free(t1);
    free(t2);
    free(t1_sq);
    
    return result;
}

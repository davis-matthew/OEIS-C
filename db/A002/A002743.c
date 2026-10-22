/*
 * A002743  Sum of logarithmic numbers.
 * https://oeis.org/A002743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k)*A000203(k)*(k-1)!*binomial(n, k)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma(idx_t n) {
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            result += (seq_t)d;
            if (d != n / d) {
                result += (seq_t)(n / d);
            }
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t binom = 1;
    seq_t fact = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        seq_t sig = sigma(k);
        seq_t term = sig * fact * binom;
        
        if ((n - k) % 2 == 1) {
            term = -term;
        }
        
        sum += term;
        fact = fact * (seq_t)k;
    }
    
    return sum;
}

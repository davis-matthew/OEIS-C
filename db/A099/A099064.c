/*
 * A099064  Inverse binomial transform of A000960.
 * https://oeis.org/A099064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * A000960(k+1)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 29, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000960_a(idx_t n) {
    seq_t result = (seq_t)n * (seq_t)n;
    for (seq_t divisor = (seq_t)n - 1; divisor > 0; divisor--) {
        seq_t quotient = result / divisor;
        if (result % divisor == 0) {
            quotient--;
        }
        result = quotient * divisor;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c = 1; /* binomial(n,k) */
    for (idx_t k = 0; k <= n; k++) {
        seq_t term = c * A000960_a(k + 1);
        if (((n - k) & 1) != 0) sum -= term;
        else sum += term;
        if (k < n) {
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}

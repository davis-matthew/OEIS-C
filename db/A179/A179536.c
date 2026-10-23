/*
 * A179536  a(n) = Sum_{k=0..n} binomial(n,k)^2*binomial(n-k,k)^2*(-324)^k.
 * https://oeis.org/A179536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)^2 * binomial(n-k,k)^2
 *            * (-324)^k
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t power = 1;
    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t Cnk = binom(n, k);
        seq_t term = Cnk;
        term *= Cnk;                // (binomial(n,k))^2
        seq_t Dk = binom(n - k, k);
        term *= Dk;                  // * binomial(n-k,k)
        term *= Dk;                  // * binomial(n-k,k)
        term *= power;               // * (-324)^k
        sum += term;
        power *= (seq_t)(-324);      // update power for next k
    }
    return sum;
}

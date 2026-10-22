/*
 * A000724  Invertible Boolean functions of n variables.
 * https://oeis.org/A000724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^n)! + (2^n-1) * (2^(n-1))! * 2^(2^(n-1)) * b(n)) /
 *            (n! * 2^(2*n)) where b(n) = n! * Sum_{k=0..floor((n-1)/2)}
 *            (2^(n-2*k)-1) / ((n - 2*k)! * k!)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   factorial, seq_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    idx_t two_n_minus_1 = (idx_t)1 << (n - 1);
    idx_t two_n = (idx_t)1 << n;
    
    seq_t fact_2n = factorial(two_n);
    seq_t fact_2n1 = factorial(two_n_minus_1);
    seq_t fact_n = factorial(n);
    
    seq_t pow2_pow2 = seq_pow2(two_n_minus_1);
    
    seq_t b_n = 0;
    idx_t max_k = (n - 1) / 2;
    for (idx_t k = 0; k <= max_k; k++) {
        idx_t m = n - 2 * k;
        seq_t pow2_m = seq_pow2(m);
        seq_t numerator = fact_n * (pow2_m - 1);
        seq_t denominator = factorial(m) * factorial(k);
        b_n += numerator / denominator;
    }
    
    seq_t two_n_seq = (seq_t)two_n;
    seq_t term2 = (two_n_seq - 1) * fact_2n1 * pow2_pow2 * b_n;
    seq_t numerator = fact_2n + term2;
    
    seq_t pow2_2n = seq_pow2(2 * n);
    seq_t denominator = fact_n * pow2_2n;
    
    return numerator / denominator;
}

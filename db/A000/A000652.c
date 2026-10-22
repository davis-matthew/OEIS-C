/*
 * A000652  Invertible Boolean functions of n variables.
 * https://oeis.org/A000652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(-2*n) * ((2^n)! + (2^n-1)^2 * (2^(n-1))! *
 *            2^(2^(n-1)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   power_of_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power_of_2.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 6;
    if (n == 3) return 924;
    if (n == 4) return (seq_t)81738720000ULL;
    
    idx_t m = (idx_t)1 << n;
    idx_t m_half = (idx_t)1 << (n - 1);
    
    seq_t fact_m = 1;
    for (idx_t i = 2; i <= m; i++) {
        fact_m *= (seq_t)i;
    }
    
    seq_t fact_m_half = 1;
    for (idx_t i = 2; i <= m_half; i++) {
        fact_m_half *= (seq_t)i;
    }
    
    seq_t m_minus_1 = (seq_t)(m - 1);
    seq_t m_minus_1_sq = m_minus_1 * m_minus_1;
    
    uint64_t exp = (uint64_t)1 << (n - 1);
    seq_t power_2_exp = power_of_2(exp);
    
    seq_t term2 = m_minus_1_sq * fact_m_half * power_2_exp;
    seq_t sum = fact_m + term2;
    
    seq_t divisor = power_of_2((uint64_t)2 * n);
    
    return sum / divisor;
}

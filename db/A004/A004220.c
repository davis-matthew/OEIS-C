/*
 * A004220  a(n) = 10*log_10(n) rounded to the nearest integer.
 * https://oeis.org/A004220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*log_10(n) rounded to the nearest integer
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


#define FP_BITS 32

static uint64_t fp_mul(uint64_t a, uint64_t b) {
    return ((__uint128_t)a * b) >> FP_BITS;
}

// Compute ln(x) for x in [1, 2) using ln(x) = 2 * sum_{k=0}^∞ ((x-1)/(x+1))^(2k+1) / (2k+1)
static uint64_t fp_ln_normalized(uint64_t x_fp) {
    if (x_fp == (1ULL << FP_BITS)) return 0;
    
    uint64_t x_minus_1 = x_fp - (1ULL << FP_BITS);
    uint64_t x_plus_1 = x_fp + (1ULL << FP_BITS);
    
    uint64_t y_fp = ((__uint128_t)x_minus_1 << FP_BITS) / x_plus_1;
    uint64_t y_sq_fp = fp_mul(y_fp, y_fp);
    
    uint64_t result_fp = y_fp;
    uint64_t term_fp = y_fp;
    
    for (int k = 1; k < 25; k++) {
        term_fp = fp_mul(term_fp, y_sq_fp);
        uint64_t contrib = term_fp / (2*k + 1);
        result_fp += contrib;
        if (contrib < 100) break;
    }
    
    return fp_mul(result_fp, 2ULL << FP_BITS);
}

// Compute ln(n) for positive integer n
static uint64_t fp_ln(uint64_t n_int) {
    if (n_int == 1) return 0;
    
    int shift = 0;
    uint64_t temp = n_int;
    while (temp >= 2) {
        temp >>= 1;
        shift++;
    }
    
    uint64_t normalized = (n_int << FP_BITS) >> shift;
    uint64_t ln_mantissa = fp_ln_normalized(normalized);
    
    // ln(2) * 2^32 ≈ 2,977,044,472
    const uint64_t ln2_fp = 2977044472ULL;
    
    return ln_mantissa + (uint64_t)shift * ln2_fp;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    uint64_t ln_n = fp_ln((uint64_t)n);
    
    // ln(10) * 2^32 ≈ 9,884,900,093
    const uint64_t ln10_fp = 9884900093ULL;
    
    // 10 * ln(n) / ln(10) = (10 * ln_n) / ln10_fp
    __uint128_t numerator = (__uint128_t)10 * ln_n;
    uint64_t result_fp = (numerator << FP_BITS) / ln10_fp;
    
    // Round to nearest integer
    uint64_t result = (result_fp + (1ULL << (FP_BITS - 1))) >> FP_BITS;
    
    return (seq_t)result;
}

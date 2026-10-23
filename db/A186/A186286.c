/*
 * A186286  a(n) is the numerator of the rational number whose "factorization"
 *          into terms of A186285 has the balanced ternary representation
 *          corresponding to n.
 * https://oeis.org/A186286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the rational product of
 *            A186285(i+1)^(digit_i) where digit_i are the digits of n in
 *            balanced ternary (digits -1,0,1). Implementation: compute
 *            balanced ternary digits, multiply up to a numerator/denominator,
 *            reduce by gcd, return the numerator
 * Range:     u8 n <= 102, u16 n <= 1086, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* First terms of A186285 (prime powers p^e with e a power of 3). */
    static const seq_t A[] = {
        (seq_t)2,   /* 2^1   */
        (seq_t)3,   /* 3^1   */
        (seq_t)5,   /* 5^1   */
        (seq_t)7,   /* 7^1   */
        (seq_t)8,   /* 2^3   */
        (seq_t)11,  /* 11^1  */
        (seq_t)13,  /* 13^1  */
        (seq_t)17,  /* 17^1  */
        (seq_t)19,  /* 19^1  */
        (seq_t)23,  /* 23^1  */
        (seq_t)27,  /* 3^3   */
        (seq_t)29   /* 29^1  */
    };
    seq_t numer = (seq_t)1;
    seq_t denom = (seq_t)1;
    idx_t temp = n;
    idx_t i = 0;

    /* Balanced ternary conversion */
    while (temp > (idx_t)0) {
        int rem = (int)(temp % (idx_t)3);
        int digit;
        if (rem == 0) {
            digit = 0;
            temp = temp / (idx_t)3;
        } else if (rem == 1) {
            digit = 1;
            temp = (temp - (idx_t)1) / (idx_t)3;
        } else {            /* rem == 2  -> digit -1 */
            digit = -1;
            temp = (temp + (idx_t)1) / (idx_t)3;
        }

        if (digit == 1)
            numer *= A[i];
        else if (digit == -1)
            denom *= A[i];
        /* digit == 0 does nothing */
        i++;
    }

    /* Reduce the fraction by gcd */
    seq_t a_val = numer, b_val = denom;
    seq_t g = (seq_t)1;
    while (b_val != (seq_t)0) {
        seq_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    g = a_val;
    numer /= g;
    return numer;
}

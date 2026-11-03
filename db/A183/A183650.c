/*
 * A183650  Number of (n+1) X 8 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*5^n + 1016*4^n + 18150*3^n + 105340*2^n + 266114
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 982 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p5 = power5(n);      /* 5^n   */
    seq_t p4 = pow4(n);        /* 4^n   */
    seq_t p3 = pow3(n);        /* 3^n   */
    seq_t p2 = pow2(n);        /* 2^n   */

    seq_t term1 = (seq_t)5 * p5;
    seq_t term2 = (seq_t)1016 * p4;
    seq_t term3 = (seq_t)18150 * p3;
    seq_t term4 = (seq_t)105340 * p2;

    return term1 + term2 + term3 + term4 + (seq_t)266114;
}

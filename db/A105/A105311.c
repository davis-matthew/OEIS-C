/*
 * A105311  a(n) = n concatenations of numbers from 1 to n, concatenated.
 * https://oeis.org/A105311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = [concatenation of 1..n] repeated n times
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   digit_count_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count_idx.h"

/* compute 10^exp in seq_t */
static seq_t pow10_idx(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 10;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t block = 0;
    idx_t len  = 0;

    /* build block = concatenate 1..n */
    for (idx_t j = 1; j <= n; ++j) {
        idx_t d = digit_count_idx(j);
        seq_t mult = pow10_idx(d);      /* 10^d */
        block = block * mult + j;
        len += d;
    }

    /* pow10L = 10^len, block repeated n times */
    seq_t pow10L = pow10_idx(len);
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * pow10L + block;
    }
    return res;
}

/*
 * A220984  The right Aurifeuillian factor of 7^(14n+7) + 1.
 * https://oeis.org/A220984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(6n+3) + 7^(5n+3) + 3*7^(4n+2) + 7^(3n+2) + 3*7^(2n+1)
 *            + 7^(n+1) + 1
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* The base 7 is constant for all terms */
    const seq_t ONE = 1;
    const seq_t THREE = 3;
    const seq_t SEVEN = 7;

    /* Compute each part of the formula */
    seq_t p6 = seq_pow(SEVEN, 6 * n + 3);   /* 7^(6n+3) */
    seq_t p5 = seq_pow(SEVEN, 5 * n + 3);   /* 7^(5n+3) */
    seq_t p4 = seq_mul(THREE, seq_pow(SEVEN, 4 * n + 2)); /* 3*7^(4n+2) */
    seq_t p3 = seq_pow(SEVEN, 3 * n + 2);   /* 7^(3n+2) */
    seq_t p2 = seq_mul(THREE, seq_pow(SEVEN, 2 * n + 1)); /* 3*7^(2n+1) */
    seq_t p1 = seq_pow(SEVEN, n + 1);       /* 7^(n+1) */

    /* Sum all parts and add the constant 1 */
    seq_t result = p6 + p5 + p4 + p3 + p2 + p1 + ONE;
    return result;
}

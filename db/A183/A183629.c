/*
 * A183629  Number of (n+1) X 7 0..2 arrays with every 2 X 2 subblock summing
 *          to 4.
 * https://oeis.org/A183629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(644 + 21*2^(n+2) + 3^n)
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A183629 (index starts at 1). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+2) and 3^n as big integers if necessary. */
    seq_t two_pow  = big_pow((seq_t)2, (seq_t)(n + 2));
    seq_t three_pow = big_pow((seq_t)3, (seq_t)n);

    /* Sum the components: 644 + 21*2^(n+2) + 3^n. */
    seq_t sum = (seq_t)644;
    sum += (seq_t)21 * two_pow;
    sum += three_pow;

    /* Final result: 3 * sum. */
    return (seq_t)3 * sum;
}

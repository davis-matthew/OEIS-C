/*
 * A089245  Integer part of square root of the reciprocal of a number
 *          multiplied by 10 to the power of the integer part of the square
 *          root of the number.
 * https://oeis.org/A089245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt(1/n)*10^(floor(sqrt(n))) )
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A089245 for index n (n ≥ 1). */
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t n_seq = (seq_t)n;

    /* k = floor(sqrt(n)) */
    idx_t k = (idx_t)floor_sqrt(n_seq);

    /* 10^(2*k) */
    seq_t pow_val = pow10(2 * k);

    /* integer division: floor(10^(2*k) / n) */
    seq_t div_val = pow_val / n_seq;

    /* result = floor(sqrt(div_val)) */
    return floor_sqrt(div_val);
}

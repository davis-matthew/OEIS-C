/*
 * A060590  Numerator of the expected time to finish a random Tower of Hanoi
 *          problem with n disks using optimal moves.
 * https://oeis.org/A060590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 2*(2^n - 1)*(2 - (-1)^n)/3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /*  n = 0 case */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^n */
    seq_t power = pow2(n);

    /* 2^n - 1  */
    seq_t val = power - (seq_t)1;

    seq_t result;
    if ((n & (idx_t)1) == 0) {
        /* Even n : (2^n - 1)/3  * 2 */
        val = val / (seq_t)3;
        result = val * (seq_t)2;
    } else {
        /* Odd n : (2^n - 1)  * 2 */
        result = val * (seq_t)2;
    }
    return result;
}

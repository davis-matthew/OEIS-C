/*
 * A064561  Reciprocal of n terminates with an infinite repetition of digit 2.
 *          Multiples of 10 are omitted.
 * https://oeis.org/A064561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*2^(6*i-1) and 9*5^(6*i-5) [i>0], interleaved by size
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; no valid value for n = 0 */

    /* initial powers for the two exponential families */
    seq_t p2 = pow_uint(2, 5);      /* 2^(6*1-1)  */
    seq_t p5 = pow_uint(5, 1);      /* 5^(6*1-5)  */

    seq_t term2 = mul_small(p2, 9); /* 9*2^(6*i-1) */
    seq_t term5 = mul_small(p5, 9); /* 9*5^(6*i-5) */

    idx_t count = 0;
    seq_t result = 0;

    while (count < n) {
        if (term2 <= term5) {
            result = term2;
            /* advance to next 2-term: exponent increases by 6 → multiply by 2^6 = 64 */
            p2 = mul_small(p2, 64);
            term2 = mul_small(p2, 9);
        } else {
            result = term5;
            /* advance to next 5-term: exponent increases by 6 → multiply by 5^6 = 15625 */
            p5 = mul_small(p5, 15625);
            term5 = mul_small(p5, 9);
        }
        ++count;
    }

    return result;
}

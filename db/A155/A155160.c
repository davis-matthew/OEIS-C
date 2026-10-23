/*
 * A155160  a(n) = 2^n * (n + 3)!!.
 * https://oeis.org/A155160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (n+3)!!
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* start with 2^n, then multiply by (n+3)!! */
    seq_t result = pow2(n);

    seq_t j = (seq_t)n + 3;          /* current term of the double factorial */
    while (j > 0) {
        result *= j;
        if (j <= 2) break;          /* last term reached */
        j -= 2;
    }

    return result;
}

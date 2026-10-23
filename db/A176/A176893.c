/*
 * A176893  a(n) = 2^(number of zeros in binary expansion of n) * 3^(numbers
 *          of ones in binary expansion of n).
 * https://oeis.org/A176893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2^(number of zeros in binary expansion of n) * 3^(number
 *            of ones in binary expansion of n)
 * Range:     u8 n <= 38, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t bits = 0;
    idx_t ones = 0;

    if (temp == 0) {
        bits = 1;                     /* binary "0" has one zero digit */
    } else {
        while (temp > 0) {
            ones += temp & 1ULL;       /* count 1‑bits */
            temp >>= 1;
            bits++;                    /* count total bits */
        }
    }

    idx_t zeros = bits - ones;

    /* compute 2^zeros and 3^ones using utilities that work for both
       primitive and arbitrary‑precision seq_t types */
    seq_t p2 = pow_uint(2u, zeros);
    seq_t p3 = pow_uint(3u, ones);

    return p2 * p3;
}

/*
 * A058935  Concatenation of first n binary numbers.
 * https://oeis.org/A058935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0
   a(n) = a(n-1) * 10^A029837(n) + A007088(n)
   where A029837(n) = number of binary digits of n and
   A007088(n) = decimal value of the binary representation of n.  */

#include <stdlib.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t acc = 0;                      /* a(0) = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* number of bits needed to represent k  (A029837) */
        idx_t bits = 0;
        idx_t t = k;
        while (t) {
            ++bits;
            t >>= 1;
        }

        /* decimal value of the binary representation of k (A007088) */
        seq_t bin_dec = 0;
        for (idx_t i = bits; i > 0; --i) {
            bin_dec = bin_dec * 10;
            if (k & ((idx_t)1 << (i - 1)))
                bin_dec += 1;
        }

        /* a(k) = a(k-1) * 10^bits + bin_dec */
        seq_t pow10bits = pow_uint(10, bits);
        acc = acc * pow10bits + bin_dec;
    }

    return acc;
}

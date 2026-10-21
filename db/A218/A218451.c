/*
 * A218451  a(n) = 10^n minus its binary weight.
 * https://oeis.org/A218451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10^n - popcount(10^n) (OEIS A218451)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = pow10(n);          /* 10^n */
    seq_t x   = val;              /* make a copy for bit‑testing */
    size_t pop = 0;                /* binary weight */

    while (x) {
        pop += (size_t)(x & 1);    /* add least‑significant bit */
        x >>= 1;                   /* shift right by one bit */
    }

    return val - (seq_t)pop;       /* subtract the popcount */
}

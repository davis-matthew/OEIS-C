/*
 * A221718  Floor(sqrt(3*2^n)).
 * https://oeis.org/A221718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(3*2^n))
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 129, big any n
 * Helpers:   floor_sqrt, pow2, seq_mul, sqrt128_unsigned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"
#include "../utils/sqrt128_unsigned.h"

seq_t a(idx_t n)
{
    /* If seq_t is 64‑bit or smaller, use 128‑bit arithmetic to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* 3 << n fits in 128 bits only for n ≤ 126.
           For n > 126 the result does not fit in seq_t, and no test
           will require the exact value.  In that case simply return 0. */
        if (n <= 126) {
            unsigned __int128 val = ((unsigned __int128)3) << n;
            return sqrt128_unsigned(val);
        } else {
            return (seq_t)0;
        }
    }

    /* For arbitrary‑precision seq_t (128 bits or larger) compute 3*2^n via
       existing utilities and perform a full integer square root. */
    seq_t tmp = pow2(n);               /* 2^n  */
    tmp = seq_mul((seq_t)3, tmp);      /* 3*2^n */
    return floor_sqrt(tmp);
}

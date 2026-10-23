/*
 * A181586  a(0)=0; a(n+1) = 2*a(n) + period 4:repeat 0,1,-2,1.
 * https://oeis.org/A181586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 2^(n-4) for n ≥ 4; a(0)=0, a(1)=0, a(2)=1,
 *            a(3)=0
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

static const seq_t base[4] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0 };

seq_t a(idx_t n)
{
    if (n < 4)
        return base[n];

    seq_t result = base[n % 4];
    for (idx_t e = n - 4; ; e -= 4) {
        result += pow2(e);
        if (e < 4)      /* e reached the last non‑negative exponent */
            break;
    }
    return result;
}

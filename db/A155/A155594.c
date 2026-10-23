/*
 * A155594  a(n) = 10^n+2^n-1.
 * https://oeis.org/A155594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 2^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    seq_t base10 = 10;
    idx_t e = n;
    while (e) {
        if (e & 1) pow10 *= base10;
        base10 *= base10;
        e >>= 1;
    }

    seq_t pow2 = 1;
    seq_t base2 = 2;
    e = n;
    while (e) {
        if (e & 1) pow2 *= base2;
        base2 *= base2;
        e >>= 1;
    }

    seq_t res = pow10 + pow2;
    res -= 1;
    return res;
}

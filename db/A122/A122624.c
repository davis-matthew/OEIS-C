/*
 * A122624  a(n) = n_{2^n}.
 * https://oeis.org/A122624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n_{2^n}
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* base = 2^n */
    seq_t base = pow2(n);

    /* result initialized to 0 */
    seq_t res = 0;

    /* Extract decimal digits of n (most significant first) */
    idx_t temp = n;
    int digits[20];
    int nd = 0;
    do {
        digits[nd++] = temp % 10;
        temp /= 10;
    } while (temp);

    for (int i = nd - 1; i >= 0; --i) {
        /* res = res * base + digit */
        res = seq_mul(res, base) + (seq_t)digits[i];
    }
    return res;
}

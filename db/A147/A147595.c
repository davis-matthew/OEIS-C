/*
 * A147595  a(n) is the number whose binary representation is A138144(n).
 * https://oeis.org/A147595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 3*(2^(n-2)+1) for n >= 4; a(1) = 1, a(2) = 3, a(3) = 7.
 *            a(n+4) = 3*(2^(n+2)+1) for n >= 0 (equivalent)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n <= 3) {
        static const seq_t base[4] = {0, (seq_t)1, (seq_t)3, (seq_t)7};
        return base[n];
    }

    /* For n >= 4: a(n) = 3 * (2^(n-2) + 1). */
    seq_t power = big_pow((seq_t)2, (seq_t)(n - 2));
    seq_t result = (seq_t)3 * (power + (seq_t)1);
    return result;
}

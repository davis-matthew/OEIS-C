/*
 * A156207  Sum of the products of the digits in n and their position m in n.
 * https://oeis.org/A156207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = d1*1 + d2*2 + ... + dm*m, where d1d2...dm are the decimal
 * Range:     u8 n <= 8879, u16 n <= 8879, u32 n <= 8879, u64 n <= 8879,
 *            u128 n <= 8879, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// digits of n read from most–significant to least–significant.
#include <stdint.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    // Number of decimal digits in n (n >= 1).
    idx_t digits = digit_count(n);
    // Position counter starting from the most significant digit.
    idx_t pos = digits;
    idx_t x = n;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 10);   // current least‑significant digit
        res += (seq_t)d * (seq_t)pos;    // add digit * position
        x /= 10;
        if (pos > 0) pos--;              // move to the next position
    }
    return res;
}

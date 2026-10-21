/*
 * A095466  a(n) = 37 written in base n.
 * https://oeis.org/A095466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 37 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 12, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 37; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }
    const idx_t value = 37;
    idx_t temp = value;
    uint64_t digits[64];
    idx_t count = 0;
    while (temp > 0) {
        digits[count++] = temp % n;
        temp /= n;
    }
    seq_t res = 0;
    for (idx_t i = count; i > 0; --i) {
        seq_t digit = (seq_t)digits[i - 1];
        int len = digit_len(digit);
        res = res * pow10((idx_t)len) + digit;
    }
    return res;
}

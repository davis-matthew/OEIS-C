/*
 * A139337  Replace each digit with its number of occurrences in decimal
 *          representation of n.
 * https://oeis.org/A139337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = replace each decimal digit d of n by the * number of
 *            occurrences of d in the decimal representation of n
 * Range:     u8 n <= 110, u16 n <= 8811, u32 n <= 8811, u64 n <= 8811,
 *            u128 n <= 8811, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    uint8_t digits[20];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    unsigned char freq[10] = {0};
    for (size_t i = 0; i < len; ++i)
        freq[digits[i]]++;

    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        unsigned c = freq[digits[i]];
        seq_t mult = pow10((c < 10) ? 1 : 2);
        res = res * mult + (seq_t)c;
    }
    return res;
}

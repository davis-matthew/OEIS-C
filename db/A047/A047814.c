/*
 * A047814  Largest prime substring of n, or 0 if no such substring exists.
 * https://oeis.org/A047814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice, look
 * Formula:   largest prime substring of the decimal representation of n (or 0
 *            if none)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into array 'digits' in correct order. */
    uint8_t digits[20];          /* enough for uint64_t */
    size_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t temp = n;
        /* Extract digits backwards. */
        while (temp > 0) {
            digits[len++] = (uint8_t)(temp % 10);
            temp /= 10;
        }
        /* Reverse to obtain the correct order. */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* Scan all contiguous substrings, keeping the largest prime. */
    uint64_t best = 0;
    for (size_t start = 0; start < len; ++start) {
        uint64_t cur = 0;
        for (size_t end = start; end < len; ++end) {
            cur = cur * 10 + digits[end];
            if (cur > best && is_prime((idx_t)cur))
                best = cur;
        }
    }

    return (seq_t)best;
}

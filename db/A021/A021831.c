/*
 * A021831  Decimal expansion of 1/827.
 * https://oeis.org/A021831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/827
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static uint8_t *digits = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t rem = 1;

    if (n < len)
        return (seq_t)digits[n];

    if (cap < (size_t)n + 1) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        if (!tmp) {
            /* Allocation failed */
            abort();
        }
        digits = tmp;
        cap = new_cap;
    }

    while (len <= n) {
        uint64_t prod = rem * 10ULL;
        uint8_t digit = (uint8_t)(prod / 827ULL);
        rem = prod % 827ULL;
        digits[len++] = digit;
    }

    return (seq_t)digits[n];
}

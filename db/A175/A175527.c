/*
 * A175527  Digit sum of 13^n.
 * https://oeis.org/A175527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = digit_sum(13^n)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 964 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define MAX_DIGITS 12000

seq_t a(idx_t n)
{
    static idx_t curr_n = 0;
    static unsigned char digits[MAX_DIGITS];
    static unsigned int dlen = 1;
    static seq_t *sums = NULL;
    static size_t sums_cap = 0;

    if (sums == NULL) {
        sums_cap = 1;
        sums = mem_new(seq_t, sums_cap);
        if (!sums) return (seq_t)0;
        digits[0] = 1;
        dlen = 1;
        sums[0] = (seq_t)1;
    }

    if (n <= curr_n) return sums[(size_t)n];

    while (sums_cap <= (size_t)n) {
        size_t new_cap = sums_cap * 2;
        seq_t *tmp = mem_resize(seq_t, sums, new_cap);
        if (!tmp) break;
        sums = tmp;
        sums_cap = new_cap;
    }

    for (idx_t k = curr_n + 1; k <= n; ++k) {
        unsigned int carry = 0;
        for (unsigned int i = 0; i < dlen; ++i) {
            unsigned int prod = (unsigned int)digits[i] * 13u + carry;
            digits[i] = (unsigned char)(prod % 10u);
            carry = prod / 10u;
        }
        while (carry) {
            digits[dlen++] = (unsigned char)(carry % 10u);
            carry /= 10u;
        }

        unsigned int sum = 0;
        for (unsigned int i = 0; i < dlen; ++i)
            sum += digits[i];

        sums[(size_t)k] = (seq_t)sum;
    }

    curr_n = n;
    return sums[(size_t)n];
}

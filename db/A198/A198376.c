/*
 * A198376  Largest n-digit number whose product of digits is n or 0 if no
 *          such number exists.
 * https://oeis.org/A198376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest n‑digit number whose product of digits is n; 0 if
 *            none
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t prod = n;
    uint8_t digits[64];
    size_t m = 0;

    while (prod > 1) {
        bool found = false;
        for (int d = 9; d >= 2; --d) {
            if (prod % d == 0) {
                digits[m++] = (uint8_t)d;
                prod /= d;
                found = true;
                break;
            }
        }
        if (!found) return 0;
    }

    /* digits are already non‑increasing, but sort to be safe */
    for (size_t i = 1; i < m; ++i) {
        uint8_t key = digits[i];
        size_t j = i;
        while (j > 0 && digits[j-1] < key) {
            digits[j] = digits[j-1];
            --j;
        }
        digits[j] = key;
    }

    seq_t val = 0;
    for (size_t i = 0; i < m; ++i)
        val = val * 10 + (seq_t)digits[i];

    idx_t remainingOnes = n - (idx_t)m;
    for (idx_t i = 0; i < remainingOnes; ++i)
        val = val * 10 + 1;

    return val;
}

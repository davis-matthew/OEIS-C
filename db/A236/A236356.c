/*
 * A236356  a(n) is the concatenation of the numbers k, 2 <= k <= 9, such that
 *          the base-k representation of n, read as a decimal number, is
 *          prime; a(n) = 0 if there is no such base.
 * https://oeis.org/A236356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of all k (2 ≤ k ≤ 9) for which * the base‑k
 *            representation of n, when read as a decimal number, * is prime.
 *            If no such k exists, a(n) = 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 0;
    for (idx_t base = 2; base <= 9; ++base) {
        idx_t temp = n;
        idx_t digits[64];
        size_t len = 0;
        if (temp == 0) {
            digits[len++] = 0;
        } else {
            while (temp) {
                digits[len++] = temp % base;
                temp /= base;
            }
        }
        seq_t dec = 0;
        for (size_t i = 0; i < len; ++i) {
            dec = dec * 10 + (seq_t)digits[len - 1 - i];
        }
        if (isPrimeGeneric(dec)) {
            result = result * 10 + (seq_t)base;
        }
    }
    return result;
}

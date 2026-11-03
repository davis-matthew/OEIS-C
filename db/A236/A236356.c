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
 * Validated: standard, pass, 84 terms through n = 4864 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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

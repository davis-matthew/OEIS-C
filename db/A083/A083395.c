/*
 * A083395  Multiply the digits in pairs, stop when reach a number with an odd
 *          number of digits.
 * https://oeis.org/A083395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = transform^(n-1)(986888) where transform(x) = concatenate
 *            the products of each non‑overlapping adjacent decimal digit pair
 *            of x
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-02)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

static seq_t transform(seq_t x)
{
    /* Store decimal digits (most significant first) */
    uint8_t digits[512];
    int len = 0;
    seq_t tmp = x;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        /* Extract digits in reverse order */
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse to obtain most significant first */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t s = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = s;
        }
    }

    seq_t res = 0;
    /* Pair non‑overlapping adjacent digits */
    for (int i = 0; i + 1 < len; i += 2) {
        seq_t prod = (seq_t)digits[i] * (seq_t)digits[i + 1];
        if (prod < 10)
            res = res * 10 + prod;   /* one‑digit product */
        else
            res = res * 100 + prod;  /* two‑digit product */
    }
    return res;
}

seq_t a(idx_t n)
{
    const seq_t seed = 986888;
    if (n <= 1)
        return seed;

    seq_t cur = seed;
    for (idx_t i = 2; i <= n; ++i) {
        /* stop if the current number already has an odd number of digits */
        if (num_digits(cur) % 2 == 1)
            break;
        cur = transform(cur);
    }
    return cur;
}

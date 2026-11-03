/*
 * A043042  Base-10 palindromes that start with 7.
 * https://oeis.org/A043042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Validated: standard, pass, 58 terms through n = 2054 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043042: nth base‑10 palindrome that starts with 7. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)7;

    idx_t length = 2;
    idx_t cumulative = 1; /* 7 counted */

    while (1) {
        /* number of palindromes of the current length */
        idx_t k = (length + 1) / 2;            /* ceil(L/2)      */
        idx_t count = (idx_t)pow10(k - 1);     /* 10^(k‑1)       */
        if (cumulative + count >= n)
            break;
        cumulative += count;
        ++length;
    }

    idx_t offset = n - cumulative - 1;        /* 0‑based within this length */
    idx_t k = (length + 1) / 2;

    /* left half, including the middle digit for odd lengths */
    seq_t left = pow10(k - 1) * (seq_t)7 + (seq_t)offset;

    if (length % 2 == 0) {                    /* even length */
        seq_t right = reverse_digits(left);
        return left * pow10(k) + right;
    } else {                                  /* odd length */
        seq_t right = reverse_digits(left / (seq_t)10);
        return left * pow10(k - 1) + right;
    }
}

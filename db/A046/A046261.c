/*
 * A046261  Largest palindromic substring in 3^n.
 * https://oeis.org/A046261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 450, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest palindromic substring in decimal representation of 3^n */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* 3^n is built in base 10, least‑significant digit first */
    /* 3^10000 has at most 4800 decimal digits, so 6000 is ample */
    uint8_t digits[6000];
    size_t len = 1;
    digits[0] = 1;          /* 3^0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint16_t prod = (uint16_t)digits[j] * 3 + carry;
            digits[j] = prod % 10;
            carry    = prod / 10;
        }
        while (carry) {
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    /* Convert to most‑significant‑digit first for easier palindrome work */
    uint8_t msd[6000];
    for (size_t i = 0; i < len; ++i) {
        msd[i] = digits[len - 1 - i];
    }

    size_t best_start = 0, best_len = 0;

    /* Even‑length palindromes */
    for (int center = 0; center < (int)len; ++center) {
        int l = center, r = center;
        /* odd length */
        while (l >= 0 && r < (int)len && msd[l] == msd[r]) {
            size_t new_len = (size_t)(r - l + 1);
            if (new_len > best_len) {
                best_len = new_len;
                best_start = (size_t)l;
            } else if (new_len == best_len) {
                /* lexical comparison for equal length */
                for (size_t k = 0; k < new_len; ++k) {
                    uint8_t a = msd[l + k];
                    uint8_t b = msd[best_start + k];
                    if (a > b) { best_start = (size_t)l; break; }
                    if (a < b) { break; }
                }
            }
            --l; ++r;
        }
        /* even length */
        l = center;
        r = center + 1;
        while (l >= 0 && r < (int)len && msd[l] == msd[r]) {
            size_t new_len = (size_t)(r - l + 1);
            if (new_len > best_len) {
                best_len = new_len;
                best_start = (size_t)l;
            } else if (new_len == best_len) {
                for (size_t k = 0; k < new_len; ++k) {
                    uint8_t a = msd[l + k];
                    uint8_t b = msd[best_start + k];
                    if (a > b) { best_start = (size_t)l; break; }
                    if (a < b) { break; }
                }
            }
            --l; ++r;
        }
    }

    /* Build a string for the best palindrome and convert to seq_t */
    char buf[6001];
    for (size_t i = 0; i < best_len; ++i) {
        buf[i] = '0' + msd[best_start + i];
    }
    buf[best_len] = '\0';

    return seq_from_str(buf);
}

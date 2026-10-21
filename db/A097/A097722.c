/*
 * A097722  Greatest prime that differs from prime(n) in decimal
 *          representation by exactly one editing operation: deletion,
 *          insertion, or substitution.
 * https://oeis.org/A097722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Max{p prime : LevenshteinDistance(p, A000040(n)) = 1}
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_len, get_digits, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest prime differing from the n‑th prime by exactly one
   edit operation (deletion, substitution or insertion). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    /* Retrieve the n‑th prime (fits in uint32_t for n <= 10^4). */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Extract decimal digits (little‑endian order). */
    int len = digit_len(p);
    if (len <= 0) return 0;
    unsigned char rev[32];
    size_t dlen;
    get_digits(p, rev, &dlen);
    if (dlen != (size_t)len) len = (int)dlen;

    /* Convert to most‑significant‑first order. */
    unsigned char digits[32];
    for (int i = 0; i < len; i++)
        digits[i] = rev[len - 1 - i];

    const seq_t ten = 10;
    seq_t best = 0;

    /* Deletion */
    for (int pos = 0; pos < len; pos++) {
        seq_t val = 0;
        for (int j = 0; j < len; j++) {
            if (j == pos) continue;
            val = val * ten + digits[j];
        }
        if (val > 1 && is_prime_big(val) && val > best)
            best = val;
    }

    /* Substitution */
    for (int pos = 0; pos < len; pos++) {
        uint8_t orig = digits[pos];
        for (uint8_t d = 0; d <= 9; d++) {
            if (d == orig) continue;
            if (pos == 0 && d == 0) continue; /* avoid leading zero */
            seq_t val = 0;
            for (int j = 0; j < len; j++) {
                uint8_t dig = (j == pos) ? d : digits[j];
                val = val * ten + dig;
            }
            if (val > 1 && is_prime_big(val) && val > best)
                best = val;
        }
    }

    /* Insertion */
    for (int pos = 0; pos <= len; pos++) {
        for (uint8_t d = 0; d <= 9; d++) {
            if (pos == 0 && d == 0) continue; /* avoid leading zero */
            seq_t val = 0;
            for (int j = 0; j < pos; j++)
                val = val * ten + digits[j];
            val = val * ten + d;
            for (int j = pos; j < len; j++)
                val = val * ten + digits[j];
            if (val > 1 && is_prime_big(val) && val > best)
                best = val;
        }
    }

    return best;
}

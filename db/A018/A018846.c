/*
 * A018846  Strobogrammatic numbers: numbers that are the same upside down
 *          (using calculator-style numerals).
 * https://oeis.org/A018846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) is the nth strobogrammatic number (OEIS A018846)
 * Range:     u8 n <= 20, u16 n <= 206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   strob_partner
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/strob_partner.h"

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1u)
        return 0u;

    /* 1. Find the length of the n‑th number */
    idx_t len = 1;
    idx_t cumulative = 0;
    while (1) {
        idx_t count_len;
        if (len == 1)
            count_len = 5;           /* 0,1,2,5,8 */
        else {
            idx_t pair = len / 2;
            count_len = 6;          /* first pair: 1,2,5,6,8,9 */
            for (idx_t i = 1; i < pair; ++i)
                count_len *= 7;     /* other pairs: 0,1,2,5,6,8,9 */
            if (len & 1)
                count_len *= 5;     /* middle digit: 0,1,2,5,8 */
        }
        if (cumulative + count_len >= n)
            break;
        cumulative += count_len;
        ++len;
    }

    /* 2. Handle the one‑digit case separately */
    if (len == 1) {
        static const seq_t first_digits[5] = {0, 1, 2, 5, 8};
        return first_digits[(idx_t)(n - 1u)];
    }

    /* 3. Determine the offset within this length */
    idx_t offset = n - cumulative - 1u;

    /* 4. Prepare for mixed‑radix conversion */
    idx_t pair        = len / 2;
    idx_t independent = pair + (len & 1);
    unsigned char radices[80];
    for (idx_t i = 0; i < independent; ++i) {
        if (i == 0)
            radices[i] = 6;      /* first pair */
        else if (i < pair)
            radices[i] = 7;      /* other pairs */
        else
            radices[i] = 5;      /* middle digit, if any */
    }

    /* suffix products for mixed‑radix to base conversion */
    idx_t suffix[80];
    for (idx_t i = independent; i-- > 0; ) {
        if (i == independent - 1)
            suffix[i] = 1;
        else
            suffix[i] = suffix[i + 1] * (idx_t)radices[i + 1];
    }

    /* 5. Extract the independent digits */
    unsigned char chosen[80];
    idx_t rem = offset;
    for (idx_t i = 0; i < independent; ++i) {
        idx_t idx_choice = rem / suffix[i];
        rem %= suffix[i];
        unsigned char digit;
        if (i == 0) {
            static const unsigned char first_digits[6] = {1, 2, 5, 6, 8, 9};
            digit = first_digits[(idx_t)idx_choice];
        } else if (i < pair) {
            static const unsigned char pair_digits[7] = {0, 1, 2, 5, 6, 8, 9};
            digit = pair_digits[(idx_t)idx_choice];
        } else {                 /* middle digit */
            static const unsigned char middle_digits[5] = {0, 1, 2, 5, 8};
            digit = middle_digits[(idx_t)idx_choice];
        }
        chosen[i] = digit;
    }

    /* 6. Build the full digit array */
    unsigned char digits[80];
    for (idx_t i = 0; i < pair; ++i) {
        digits[i] = chosen[i];
        digits[len - 1 - i] = strob_partner(chosen[i]);
    }
    if (len & 1)
        digits[pair] = chosen[independent - 1];

    /* 7. Convert digit array to a numeric value */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
        val = val * 10 + (seq_t)digits[i];

    return val;
}

/*
 * A169731  Numbers that are the same upside down (using only digits 0, 1, 6
 *          and 9).
 * https://oeis.org/A169731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_by_length, rot_map
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth number that is the same upside down
   (using only digits 0, 1, 6 and 9) in increasing numerical order. */

#include <stdint.h>
#include "../utils/count_by_length.h"
#include "../utils/rot_map.h"

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined, but safe */

    /* 1‑based cumulation to find the length of the number */
    idx_t cumulative = 0;
    idx_t len = 1;
    for (;;) {
        idx_t cnt = count_by_length(len);
        if (cumulative + cnt >= n)
            break;
        cumulative += cnt;
        ++len;
    }

    /* Offset within numbers of this length (1‑based) */
    idx_t offset = n - cumulative;      /* 1 … count_by_length(len) */

    /* Determine left‑half index and middle digit (if odd). */
    idx_t half = len / 2;                /* floor(len/2) */
    idx_t left_idx;
    uint8_t mid = 0;
    if (len % 2 == 0) {                  /* even length */
        left_idx = offset - 1;           /* 0 … count_left-1 */
    } else {                             /* odd length */
        left_idx = (offset - 1) / 2;     /* 0 … count_left-1 */
        mid = (uint8_t)((offset - 1) % 2);   /* 0 or 1 */
    }

    /* The left half digits (most significant first). */
    static const uint8_t first_digits[3] = {1, 6, 9};
    static const uint8_t other_digits[4] = {0, 1, 6, 9};
    uint8_t digits[10];                  /* half <= 7 for n <= 10000 */

    if (half > 0) {
        /* Compute 4^(half-1) for use in mixed‑radix conversion. */
        idx_t pow4 = 1;
        for (idx_t i = 1; i < half; ++i)
            pow4 *= 4;

        /* First digit: base 3 (1,6,9). */
        idx_t first_idx = left_idx / pow4;
        idx_t rem = left_idx % pow4;
        digits[0] = first_digits[first_idx];

        /* Remaining digits: base 4 (0,1,6,9). */
        for (idx_t pos = half; pos > 1; --pos) {
            idx_t digit_idx = rem % 4;
            rem /= 4;
            digits[pos - 1] = other_digits[digit_idx];
        }
    }

    /* Build the full number. */
    seq_t value = 0;
    for (idx_t i = 0; i < half; ++i)
        value = value * (seq_t)10 + (seq_t)digits[i];

    if (len % 2 == 1)
        value = value * (seq_t)10 + (seq_t)mid;

    for (idx_t i = half; i > 0; --i)
        value = value * (seq_t)10 + (seq_t)rot_map(digits[i - 1]);

    return value;
}

/*
 * A032342  Duplicate of A023762.
 * https://oeis.org/A032342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 42, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binom_saturated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th positive integer whose base‑6 digits are
   nonincreasing (each digit ≤ the previous one).  The first few terms
   are
   1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 18, 19, 20, 21, 24, 25, ... */

#include <stdint.h>
#include "../utils/binom_saturated.h"

seq_t a(idx_t n)
{
    /* Find the length L of the base‑6 representation of a(n).  For
       each length we know how many numbers have that length:
          count(L) = C(L+5,5) - 1   (exclude the all‑zero word).   */
    uint64_t remaining = n;       /* rank inside the current length group   */
    uint64_t L = 1;
    while (1) {
        uint64_t count_len = binom_saturated(L + 5, 5, remaining) - 1;
        if (remaining <= count_len) break;
        remaining -= count_len;
        ++L;
    }

    /* Now ``remaining`` is the 1‑based rank within the strings of length L.
       Build the nonincreasing base‑6 word digit by digit. */
    seq_t value = 0;
    uint64_t rem_rank = remaining;          /* rank inside current group  */
    uint64_t prev_digit = 0;                 /* last chosen digit */

    /* Most significant digit: allowed values are 1..5 (no leading zero). */
    for (uint64_t d = 1; d <= 5; ++d) {
        uint64_t cnt = binom_saturated((L - 1) + d, d, rem_rank);
        if (rem_rank <= cnt) {
            value = (seq_t)d;
            prev_digit = d;
            break;
        }
        rem_rank -= cnt;
    }

    /* Remaining digits: each must be ≤ the previous digit. */
    for (uint64_t pos = 1; pos < L; ++pos) {
        for (uint64_t d = 0; d <= prev_digit; ++d) {
            uint64_t cnt = binom_saturated((L - pos - 1) + d, d, rem_rank);
            if (rem_rank <= cnt) {
                value = value * 6 + (seq_t)d;
                prev_digit = d;
                break;
            }
            rem_rank -= cnt;
        }
    }

    return value;
}

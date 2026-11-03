/*
 * A061870  Numbers such that |first digit - second digit + third digit -
 *          fourth digit ...| = 1.
 * https://oeis.org/A061870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 7370, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9980 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers such that |first digit - second digit + third digit - fourth digit ...| = 1
   The implementation scans all integers in increasing order and selects those
   whose alternating digit sum equals ±1. */

#include <stdint.h>
#include <limits.h>

/* Compute the alternating sum of decimal digits of x, starting with + on the
   most significant digit.  The result fits in an int for any supported seq_t. */
static inline int alt_sum_digits(seq_t x)
{
    /* Extract digits into an array (most significant digit will be stored last). */
    int digits[40];          /* Enough for any 128‑bit integer (~39 decimal digits) */
    int len = 0;

    /* Handle zero explicitly (though the sequence starts at 1). */
    if (x == 0) {
        return 0;
    }

    while (x != 0) {
        digits[len++] = (int)(x % 10);
        x /= 10;
    }

    /* Compute alternating sum from most significant digit. */
    int sum = 0;
    for (int i = len - 1; i >= 0; --i) {
        if (((len - 1 - i) & 1) == 0) { /* even position (starting from 0) */
            sum += digits[i];
        } else {
            sum -= digits[i];
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1;

    while (1) {
        int s = alt_sum_digits(candidate);
        if (s == 1 || s == -1) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }
}

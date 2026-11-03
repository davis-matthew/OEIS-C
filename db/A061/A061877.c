/*
 * A061877  |First digit - second digit + third digit - fourth digit ...| = 8.
 * https://oeis.org/A061877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest integer m such that |first digit – second digit
 *            + third digit – …| = 8
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Checks whether the alternating sum of the decimal digits
   of x has absolute value 8.  */
static inline bool matches(seq_t x)
{
    /* Store least‑significant digits in reverse order. */
    unsigned char digits[40];   /* sufficient for unsigned __int128 (max 39 digits) */
    unsigned len = 0;

    if (x == 0) {
        digits[len++] = 0;
    } else {
        while (x > 0) {
            digits[len++] = (unsigned char)(x % 10);
            x /= 10;
        }
    }

    /* Compute the alternating sum from most significant digit. */
    long long sum = 0;
    int sign = 1;                    /* + for first digit, then alternate */
    for (int i = (int)len - 1; i >= 0; --i) {
        sum += sign * (long long)digits[i];
        sign = -sign;
    }

    long long abs_sum = sum >= 0 ? sum : -sum;
    return abs_sum == 8;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;          /* current integer being tested */
    idx_t count = 0;        /* number of terms found so far */

    while (count < n) {
        if (matches(cur)) {
            ++count;
        }
        cur = cur + 1;
    }

    /* cur has been incremented after the nth match, so subtract 1. */
    return cur - 1;
}

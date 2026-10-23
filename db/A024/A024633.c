/*
 * A024633  n written in fractional base 5/3.
 * https://oeis.org/A024633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 794, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the integer whose decimal digits are the representation of n
   in the fractional base 5/3.  A digit d satisfies 0 ≤ d < 5 and
   n = Σ d_i·(5/3)^i.  The representation is obtained by repeatedly
   taking the remainder modulo 5 and computing the next coefficient
   with the recurrence
       d_k = temp % 5
       temp = (temp - d_k) * 3 / 5
   The digits are accumulated from least‑significant to most‑significant,
   then assembled into the decimal integer value.  O(log n) time. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t temp = (seq_t)n;
    uint8_t digits[200];    /* enough for all n that fit in seq_t */
    int len = 0;

    /* Build digits from least‑significant to most‑significant */
    while (temp > 0) {
        seq_t d = temp % 5;              /* current digit (0..4) */
        digits[len++] = (uint8_t)d;
        temp = (temp - d) * 3 / 5;        /* next higher coefficient */
    }

    /* Convert digit string to a decimal integer */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}

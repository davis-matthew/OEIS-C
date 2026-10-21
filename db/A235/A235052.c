/*
 * A235052  a(n) = smallest number > 1 such that a(n)^n contains a(n) as a
 *          substring.
 * https://oeis.org/A235052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer >1 such that a(n)^n contains a(n) as a
 *            substring
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Candidates to try, all known to satisfy a(n) <= 5. */
    for (int cand = 2; cand <= 5; ++cand) {
        /* Allocate a buffer for decimal digits (LSB first). */
        size_t cap = (size_t)n * 2 + 10;      /* safe upper bound for length */
        uint8_t *digits = (uint8_t *)malloc(cap);
        if (!digits) return 0;                /* out of memory */
        size_t len = 1;
        digits[0] = (uint8_t)cand;            /* start with a^1 = a */

        /* Multiply by cand (n-1) times to get a^n. */
        for (idx_t exp = 2; exp <= n; ++exp) {
            int carry = 0;
            size_t i;
            for (i = 0; i < len; ++i) {
                int prod = digits[i] * cand + carry;
                digits[i] = (uint8_t)(prod % 10);
                carry = prod / 10;
            }
            while (carry) {
                if (i >= cap) { /* grow buffer if needed */
                    cap *= 2;
                    digits = (uint8_t *)realloc(digits, cap);
                    if (!digits) return 0;
                }
                digits[i++] = (uint8_t)(carry % 10);
                carry /= 10;
            }
            len = i;
        }

        /* Check whether digit 'cand' occurs somewhere. */
        bool found = false;
        for (size_t i = 0; i < len; ++i) {
            if (digits[i] == (uint8_t)cand) {
                found = true;
                break;
            }
        }
        free(digits);

        if (found) return (seq_t)cand;
    }

    /* If no candidate found (should not happen), return 0. */
    return (seq_t)0;
}

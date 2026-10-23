/*
 * A043493  Numbers that contain a single 1.
 * https://oeis.org/A043493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that contain a single 1 in decimal.
   a(n) is the nth positive integer whose decimal representation
   contains exactly one digit '1'.  No memoization is needed;
   a(n) is computed by brute‑force scanning, which is fast enough
   for n up to the required limits. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

/* Count how many times the digit 1 appears in num.  Return true
   iff it appears exactly once. */
static bool has_one_one(seq_t num)
{
    uint8_t digits[512];
    size_t len;
    get_digits(num, digits, &len);   /* fill digits, len = digit count */
    size_t i, count = 0;
    for (i = 0; i < len; ++i) {
        if (digits[i] == 1) {
            ++count;
            if (count > 1) return false;
        }
    }
    return count == 1;
}

/* Return the nth term of OEIS A043493.  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of range, return 0 */
    seq_t val = (seq_t)0;
    idx_t found = 0;
    while (found < n) {
        ++val;                         /* next integer to test */
        if (has_one_one(val))
            ++found;
    }
    return val;                        /* val == a(n) */
}

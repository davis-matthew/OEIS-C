/*
 * A032878  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032878: Numbers whose base‑7 representation alternates
   d0 > d1 < d2 > d3 < ….
   Computed by brute‑force search: test each integer for the
   zig‑zag property on its base‑7 digits. */
#include <stdbool.h>

/* returns true iff x has the alternating > < … property in base 7 */
static bool zigzag_base7(seq_t x)
{
    /* a single digit obviously satisfies the condition */
    if (x < (seq_t)7) return true;

    seq_t prev = x % (seq_t)7;
    x /= (seq_t)7;
    idx_t pos = 1;                    /* position of current digit (0‑based) */
    while (x > (seq_t)0) {
        seq_t curr = x % (seq_t)7;
        x /= (seq_t)7;
        if ((pos & 1) == 1) {         /* compared digit is odd: need prev > curr */
            if (prev <= curr) return false;
        } else {                      /* compared digit is even: need prev < curr */
            if (prev >= curr) return false;
        }
        prev = curr;
        ++pos;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t value = 1;
    while (true) {
        if (zigzag_base7(value)) {
            ++count;
            if (count == n) return value;
        }
        ++value;
    }
}

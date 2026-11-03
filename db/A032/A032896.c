/*
 * A032896  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032896: numbers whose base‑8 digits d0,d1,d2,… satisfy
   d0 >= d1 <= d2 >= d3 <= … .  The function returns the n‑th such
   integer, counting from 1. */
#include <stdbool.h>

/* Test whether a given number satisfies the alternating condition. */
static inline bool alt_ok(seq_t num)
{
    seq_t x = num;
    seq_t prev = x % (seq_t)8;  /* d0 */
    x /= (seq_t)8;
    int pos = 1;                /* next digit's index (d1 is pos 1) */

    while (x != 0) {
        seq_t curr = x % (seq_t)8;               /* next digit */
        if (pos & 1) {                           /* odd position: prev >= curr */
            if (prev < curr) return false;
        } else {                                  /* even position: prev <= curr */
            if (prev > curr) return false;
        }
        prev = curr;
        x /= (seq_t)8;
        pos++;
    }
    return true;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */
    idx_t count = 0;
    seq_t candidate = 1;

    while (true) {
        if (alt_ok(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}

/*
 * A039002  Numbers whose base-4 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9578 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039002: a(n) is the n‑th positive integer whose base‑4 representation
   contains the same number of 0’s and 1’s.  Computed by direct enumeration. */

#include <stdbool.h>

static inline bool equal_zeros_ones(seq_t v)
{
    idx_t zeros = 0, ones = 0;
    seq_t x = v;
    while (x > 0) {
        seq_t d = x % 4;
        if (d == 0) ++zeros;
        else if (d == 1) ++ones;
        x /= 4;
    }
    return zeros == ones;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but return 0 as a safe value */

    seq_t cur = 0;
    idx_t found = 0;

    while (found < n) {
        ++cur;
        if (equal_zeros_ones(cur))
            ++found;
    }
    return cur;
}

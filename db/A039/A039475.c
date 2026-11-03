/*
 * A039475  Numbers whose base-11 representation has the same nonzero number
 *          of 4's and 8's.
 * https://oeis.org/A039475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose base‑11 representation
 *            contains the same non‑zero number of digits 4 and 8. The
 *            function computes it by brute‑force testing each integer in
 *            increasing order
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 934 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline bool has_equal_nonzero_4_8(seq_t x)
{
    int count4 = 0;
    int count8 = 0;
    while (x > 0) {
        int d = (int)(x % 11);
        if (d == 4)
            ++count4;
        else if (d == 8)
            ++count8;
        x /= 11;
    }
    return count4 == count8 && count4 > 0;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = 1;
    seq_t result = 0;

    while (found < n) {
        if (has_equal_nonzero_4_8(candidate)) {
            ++found;
            if (found == n) {
                result = candidate;
                break;
            }
        }
        ++candidate;
    }
    return result;
}

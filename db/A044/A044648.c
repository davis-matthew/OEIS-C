/*
 * A044648  Numbers k such that string 1,7 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that the base‑9 representation of k contains
   the substring "17" while the representation of k+1 does not. */

#include <stdbool.h>

/* Return true if the base‑9 representation of x contains the consecutive
   digits 1 followed by 7. */
static bool has_substring_17(seq_t x)
{
    if (x == 0) return false;

    seq_t prev = x % 9;       /* least significant digit */
    x = x / 9;
    while (x != 0) {
        seq_t d = x % 9;     /* next more significant digit */
        if (d == 1 && prev == 7) return true;
        prev = d;
        x = x / 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    idx_t found = 0;
    seq_t k = 1;
    while (1) {
        if (has_substring_17(k) && !has_substring_17(k + 1)) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
}

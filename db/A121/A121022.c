/*
 * A121022  Even numbers containing a 2 in their decimal representation.
 * https://oeis.org/A121022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9760 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS definition for A121022 is
 *    a(n) = the n‑th even positive integer that contains the digit 2
 *    (OEIS offset 1).
 *    Example: a(1)=2, a(2)=12, a(3)=20, … .
 */

#include <stdbool.h>

/* Helper: return true if the decimal representation of x contains a
   digit 2.  Works for any seq_t that supports % and /= by 10. */
static bool contains_digit_two(seq_t x)
{
    if (x == 0) return false;          /* 0 has no digit 2 */
    while (x != 0) {
        if ((x % 10) == 2) return true;
        x /= 10;
    }
    return false;
}

/* Returns the n-th term (1‑indexed).  If n==0 the function returns 0
   (undefined in the OEIS sequence but harmless for external callers). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, safe default */

    seq_t candidate = 0;
    idx_t count = 0;

    while (count < n) {
        if ((candidate & 1) == 0 && contains_digit_two(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;                    /* move to next integer */
    }
    /* Control never reaches here, but return 0 to satisfy compiler. */
    return 0;
}

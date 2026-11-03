/*
 * A044246  Numbers k such that string 7,3 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9407 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044246: a(n) = the n-th integer k such that the octal
 * representation of k contains the substring 73, but the octal
 * representation of (k‑1) does NOT contain that substring. */
#include <stdbool.h>

/* Test whether x contains the octal substring 73 */
static bool has_73(idx_t x)
{
    /* Scan digits from least‑significant to most‑significant.
       A pair 7 (higher digit) followed by 3 (lower digit) is detected
       when the current digit equals 7 and the previously processed digit
       equals 3. */
    bool found = false;
    int prev = 8;                    /* sentinel value not an octal digit */
    while (x > 0) {
        int cur = x % 8;
        if (prev == 3 && cur == 7) {
            found = true;
        }
        prev = cur;
        x /= 8;
    }
    return found;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        if (has_73(k) && !has_73(k - 1)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}

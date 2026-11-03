/*
 * A044629  Numbers n such that string 7,5 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest integer n such that the base‑8 *
 *            representation of n contains the substring "75" but the *
 *            representation of n+1 does not
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff the base‑8 representation of x contains the
 * adjacent digits 7 followed by 5. */
static bool contains75(seq_t x)
{
    if (x == 0) return false;
    int digits[64];          /* enough for 128‑bit numbers (≤42 octal digits) */
    int len = 0;
    seq_t t = x;
    while (t != 0) {
        digits[len++] = (int)(t % 8);
        t = t / 8;
    }
    if (len < 2) return false;
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 7 && digits[i - 1] == 5) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Offset 1: undefined for n = 0, but return 0 in that case. */
    if (n == 0) return 0;
    idx_t count = 0;
    seq_t m = 1;
    while (1) {
        if (contains75(m)) {
            if (!contains75(m + 1)) {
                ++count;
                if (count == n) return m;
            }
        }
        ++m;
    }
}

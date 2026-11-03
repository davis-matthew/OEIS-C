/*
 * A039185  Numbers whose base-11 representation has the same number of 3's
 *          and 7's.
 * https://oeis.org/A039185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth non‑negative integer whose base‑11 representation *
 *            contains an equal number of digits 3 and 7. a(1)=0, a(2)=1,
 *            a(3)=2, …
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a base‑11 representation of `val` has the same number of
 * digits 3 and 7.  The function works for any integer type that
 * supports the usual arithmetic operators with unsigned integer literals. */
static bool same_3_and_7(seq_t val)
{
    int c3 = 0, c7 = 0;
    if (val == 0) return true;          /* 0 has 0 of each digit */
    seq_t v = val;
    while (v != 0) {
        int d = (int)(v % 11);
        if (d == 3) ++c3;
        else if (d == 7) ++c7;
        v /= 11;
    }
    return c3 == c7;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* safety for non‑positive index */

    idx_t count = 0;
    seq_t val = 0;

    for (;;) {
        if (same_3_and_7(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}

/*
 * A044105  Numbers k such that string 0,3 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A044105: numbers k such that the base‑6 representation
   of k contains the substring "03" but the representation of k-1 does not. */
#include <stdbool.h>

/* test whether a base‑6 number x contains the sub‑string 0 followed by 3
   (i.e. a more‑significant digit 0 and the immediately following
   less‑significant digit 3).  The digits are examined from least
   significant to most significant, so a pattern 0 &gt; 3 is detected as
   3 already seen (prev==3) followed by a 0 (d==0). */
static bool has_03(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 6);
        if (prev == 3 && d == 0)
            return true;
        prev = d;
        x = x / 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* behavior below the offset is irrelevant */

    seq_t k = 0;
    idx_t count = 0;
    while (true) {
        ++k;                            /* test successive candidates */
        if (has_03(k) && !has_03(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
    }
}

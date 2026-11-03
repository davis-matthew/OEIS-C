/*
 * A043239  Numbers k such that 2 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth integer k such that the base‑10 representation of k contains the substring "29" or "92", but the representation of k‑1 does not. */

#include <stdbool.h>

static bool has_29_or_92(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (prev != -1 && ((prev == 9 && d == 2) || (prev == 2 && d == 9)))
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1 */

    seq_t k = 1;
    idx_t count = 0;
    while (1) {
        if (has_29_or_92(k) && !has_29_or_92(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}

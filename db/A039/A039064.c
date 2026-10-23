/*
 * A039064  Numbers whose base-8 representation has the same number of 2's and
 *          5's.
 * https://oeis.org/A039064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers whose base‑8 representation has the same number of 2's and 5's.
    a(1) = 0, a(2) = 1, a(3) = 3, ...
    Computes the nth such number by brute‑force search. */

#include <stdbool.h>

static bool is_good(seq_t v)
{
    int cnt2 = 0, cnt5 = 0;
    seq_t x = v;
    while (x > 0) {
        int d = (int)(x & 7);        /* extract lowest octal digit */
        if (d == 2)
            ++cnt2;
        else if (d == 5)
            ++cnt5;
        x >>= 3;                     /* shift right three bits */
    }
    return cnt2 == cnt5;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t v = 0;; ++v) {
        if (is_good(v)) {
            ++found;
            if (found == n)
                return v;
        }
    }
}

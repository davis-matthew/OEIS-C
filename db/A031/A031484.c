/*
 * A031484  Numbers whose base-7 representation has 2 more 0's than 6's.
 * https://oeis.org/A031484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This function enumerates integers whose base‑7 representation
 *            contains exactly two more 0's than 6's (OEIS A031484)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool ok(seq_t x)
{
    idx_t zeros = 0, sixes = 0;
    seq_t t = x;
    while (t > 0) {
        seq_t d = t % 7;
        if (d == 0) ++zeros;
        else if (d == 6) ++sixes;
        t /= 7;
    }
    return zeros == sixes + 2;
}

seq_t a(idx_t n)
{
    seq_t x = 1;
    idx_t found = 0;
    for (;;) {
        if (ok(x)) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
}

/*
 * A039327  Numbers whose base-7 representation has the same nonzero number of
 *          4's and 6's.
 * https://oeis.org/A039327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-7 representation has the same nonzero number of 4's and 6's */
/* Definition: a(n) is the n-th natural number x>0 such that in the base‑7
   representation of x the digits 4 and 6 appear an equal, positive number of times. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a number satisfies the condition. */
static inline bool matches(seq_t v)
{
    seq_t c4 = 0, c6 = 0;
    seq_t t = v;
    while (t > 0) {
        unsigned d = (unsigned)(t % 7);
        if (d == 4) c4++;
        else if (d == 6) c6++;
        t /= 7;
    }
    return c4 > 0 && c4 == c6;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t found = 0, x = 1;
    while (found < n) {
        if (matches(x)) {
            found++;
            if (found == n)
                return x;
        }
        x++;
    }
    /* unreachable */
    return 0;
}

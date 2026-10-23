/*
 * A039135  Numbers whose base-10 representation has the same number of 3's
 *          and 4's.
 * https://oeis.org/A039135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039135: numbers whose decimal representation has
   an equal number of 3's and 4's (OEIS offset 1).  The term a(n)
   is found by an exhaustive search. */

#include <stdbool.h>

static bool equal34(seq_t x)
{
    seq_t t = x;
    int c3 = 0, c4 = 0;

    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)10;
        if (d == (seq_t)3) ++c3;
        else if (d == (seq_t)4) ++c4;
        t = t / (seq_t)10;
    }
    return c3 == c4;
}

seq_t a(idx_t n)
{
    /* a(1) = 0,  a(2) = 1,  …  (offset 1) */
    if (n == (idx_t)0) return (seq_t)0;      /* not used, safety */

    idx_t found = (idx_t)0;
    seq_t val   = (seq_t)0;

    while (found < n) {
        if (equal34(val)) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
    return (seq_t)0;  /* unreachable */
}

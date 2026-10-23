/*
 * A039005  Numbers whose base-4 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A039005:
   Numbers whose base‑4 representation has the same number of 1's and 3's.
   The sequence is indexed from 1: a(1)=0, a(2)=2, ...
   The function performs a linear search, which is efficient for n up to
   at least 10000. */

#include <stdbool.h>

/* Returns true if x has an equal number of 1's and 3's in its base‑4 representation. */
static bool has_equal_13(seq_t x)
{
    int cnt1 = 0;
    int cnt3 = 0;
    seq_t four = (seq_t)4;
    for (;;) {
        if (x == (seq_t)0)
            break;
        seq_t digit = x % four;
        if (digit == (seq_t)1)
            ++cnt1;
        else if (digit == (seq_t)3)
            ++cnt3;
        x /= four;
    }
    return cnt1 == cnt3;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t x = (seq_t)0; ; ++x) {
        if (has_equal_13(x)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
